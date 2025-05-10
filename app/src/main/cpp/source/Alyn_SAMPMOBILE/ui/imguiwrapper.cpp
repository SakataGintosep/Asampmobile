#include "../main.h"
#include "imguiwrapper.h"
#include "uisettings.h"
#include "../settings.h"
#include "../samp.h"
#include "../java.h"
#include "../voice/include/util/Render.h"
#include "../voice/SpeakerList.h"
#include "../voice/MicroIcon.h"

extern UI* pUI;

ImGuiWrapper::ImGuiWrapper(const ImVec2& display_size, const std::string& font_path)
{
	m_displaySize = display_size;
	m_renderer = 0;
	m_fontRaster = nullptr;
	m_fontPath = font_path;

	m_vertexBuffer = nullptr;
	m_vertexBufferSize = 10000;
}

ImGuiWrapper::~ImGuiWrapper()
{
	shutdown();
}

bool ImGuiWrapper::initialize()
{
	LOGI("ImGuiWrapper::initialize");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = m_displaySize;
	io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

	LOGI("Loading font: %s", m_fontPath.c_str());

	ImFontAtlas::GlyphRangesBuilder builder;
	builder.AddRanges(io.Fonts->GetGlyphRangesDefault());
	builder.AddRanges(io.Fonts->GetGlyphRangesCyrillic());
	builder.AddRanges(io.Fonts->GetGlyphRangesChineseFull());
	builder.AddRanges(io.Fonts->GetGlyphRangesJapanese());
	builder.AddRanges(io.Fonts->GetGlyphRangesKorean());

	auto ranges = new ImVector<ImWchar>();
	builder.BuildRanges(ranges);

	ImFontConfig fontCfg;
	fontCfg.OversampleV = 1; // High Quality Pixel Positioning
	fontCfg.OversampleH = 1; // High Quality Pixel Positioning
	fontCfg.RasterizerMultiply = 1.5f;
	fontCfg.PixelSnapH = true; // Glyphs are aligned to pixel boundaries
	fontCfg.GlyphExtraSpacing.x = UISettings::outlineSize();

	ImFont* font = io.Fonts->AddFontFromFileTTF(m_fontPath.c_str(), UISettings::fontSize(), &fontCfg, ranges->Data);

	if (font == nullptr) {
		LOGE("Failed to load font %s", m_fontPath.c_str());
		return false;
	}

	std::string weap_font_path = std::string((char*) (SA_Addr(0x6D687C))) + "fonts/gtaweap3.ttf";
	m_weapFont = io.Fonts->AddFontFromFileTTF(weap_font_path.c_str(), UISettings::fontSize(), &fontCfg, ranges->Data);

	if (m_weapFont == nullptr) {
		LOGE("Failed to load font %s", weap_font_path.c_str());
		return false;
	}

	createFontTexture();

	m_renderer = new ImGuiRenderer(ImGui::GetBackgroundDrawList(), font);

	for (const auto& deviceInitCallback : Render::deviceInitCallbacks) {
		if (deviceInitCallback != nullptr) {
			deviceInitCallback();
		}
	}

	return true;
}

void ImGuiWrapper::render()
{
	ImGui::NewFrame();

	if (SAMP::netgame()) {
		if (SAMP::paused()) {
			SpeakerList::Hide();
			MicroIcon::Hide();
		}
		else {
			SpeakerList::Show();
			MicroIcon::Show();
		}

		for (const auto& renderCallback : Render::renderCallbacks) {
			if (renderCallback != nullptr) {
				renderCallback();
			}
		}
	}

	drawList();

	ImGui::EndFrame();
	ImGui::Render();
	renderDrawData(ImGui::GetDrawData());
}

void ImGuiWrapper::shutdown()
{
	LOGI("ImGuiWrapper::shutdown");
	destroyFontTexture();
}

void ImGuiWrapper::setupRenderState()
{
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*) 0);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*) 0);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*) 1);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*) rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*) rwBLENDINVSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*) 0);
	RwRenderStateSet(rwRENDERSTATECULLMODE, (void*) rwCULLMODECULLNONE);
	RwRenderStateSet(rwRENDERSTATEBORDERCOLOR, (void*) 0);
	RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void*) rwALPHATESTFUNCTIONGREATER);
	RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, (void*) 2);
	RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*) rwFILTERLINEAR);
	RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, (void*) rwTEXTUREADDRESSCLAMP);
}

void ImGuiWrapper::checkVertexBuffer(ImDrawData* draw_data)
{
	if (m_vertexBuffer == nullptr || m_vertexBufferSize < draw_data->TotalVtxCount) {
		LOGI("ImGuiWrapper::checkVertexBuffer");

		if (m_vertexBuffer) {
			delete m_vertexBuffer;
			m_vertexBuffer = nullptr;
		}

		m_vertexBufferSize = draw_data->TotalVtxCount + 10000;
		m_vertexBuffer = new RwIm2DVertex[m_vertexBufferSize];
		if (!m_vertexBuffer) {
			LOGW("Buffer allocation error. Size %d", m_vertexBufferSize);
			return;
		}

		LOGI("Vertex buffer reallocated. Size %d", m_vertexBufferSize);
	}
}

RwReal getNearScreenZ()
{
	return *(RwReal*) (SA_Addr(0xA7C348));
}

RwReal getRecipNearClip()
{
	return *(RwReal*) (SA_Addr(0xA7C344));
}

void setScissorRect(void* pRect)
{
	return ((void (*)(void*)) (SA_Addr(0x2B3E54 + 1)))(pRect);
}

void ImGuiWrapper::renderDrawData(ImDrawData* draw_data)
{
	const RwReal nearScreenZ = getNearScreenZ();
	const RwReal recipNearClip = getRecipNearClip();

	checkVertexBuffer(draw_data);
	setupRenderState();

	RwIm2DVertex* vtx_dst = m_vertexBuffer;
	int vtx_offset = 0;

	for (int n = 0; n < draw_data->CmdListsCount; n++) {
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		const ImDrawVert* vtx_src = cmd_list->VtxBuffer.Data;
		const ImDrawIdx* idx_src = cmd_list->IdxBuffer.Data;

		for (int i = 0; i < cmd_list->VtxBuffer.Size; i++) {
			RwIm2DVertexSetScreenX(vtx_dst, vtx_src->pos.x);
			RwIm2DVertexSetScreenY(vtx_dst, vtx_src->pos.y);
			RwIm2DVertexSetScreenZ(vtx_dst, nearScreenZ);
			RwIm2DVertexSetRecipCameraZ(vtx_dst, recipNearClip);
			vtx_dst->emissiveColor = vtx_src->col;
			RwIm2DVertexSetU(vtx_dst, vtx_src->uv.x, recipCameraZ);
			RwIm2DVertexSetV(vtx_dst, vtx_src->uv.y, recipCameraZ);

			vtx_dst++;
			vtx_src++;
		}

		const ImDrawIdx* idx_buffer = cmd_list->IdxBuffer.Data;
		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if (pcmd->UserCallback) {
				pcmd->UserCallback(cmd_list, pcmd);
			}
			else {
				ImVec4 clip_rect;
				clip_rect.x = pcmd->ClipRect.x;
				clip_rect.y = pcmd->ClipRect.w;
				clip_rect.z = pcmd->ClipRect.z;
				clip_rect.w = pcmd->ClipRect.y;
				setScissorRect((void*) &clip_rect);

				RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void*) pcmd->TextureId);
				RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST,
						&m_vertexBuffer[vtx_offset], (RwInt32) cmd_list->VtxBuffer.Size,
						(RwImVertexIndex*) idx_buffer, pcmd->ElemCount);
				RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void*) 0);

				clip_rect = {0.0f, 0.0f, 0.0f, 0.0f};
				setScissorRect((void*) &clip_rect);
			}

			idx_buffer += pcmd->ElemCount;
		}
		vtx_offset += cmd_list->VtxBuffer.Size;
	}
}

bool ImGuiWrapper::createFontTexture()
{
	LOGI("ImGuiWrapper::createFontTexture");

	ImGuiIO& io = ImGui::GetIO();
	unsigned char* pxs;
	int width, height, bytes_per_pixel;
	io.Fonts->GetTexDataAsRGBA32(&pxs, &width, &height, &bytes_per_pixel);

	RwImage* font_img = RwImageCreate(width, height, bytes_per_pixel * 8);
	RwImageAllocatePixels(font_img);

	if (font_img == nullptr) {
		LOGI("Font image pointer 0x%x", font_img);
		return false;
	}

	RwUInt8* pixels = font_img->cpPixels;
	for (int y = 0; y < font_img->height; y++) {
		memcpy((void*) pixels, pxs + font_img->stride * y, font_img->stride);
		pixels += font_img->stride;
	}

	RwInt32 w, h, d, flags;
	RwImageFindRasterFormat(font_img, rwRASTERTYPETEXTURE, &w, &h, &d, &flags);
	m_fontRaster = RwRasterCreate(w, h, d, flags);
	if (m_fontRaster == nullptr) {
		LOGI("font raster pointer 0x%x", m_fontRaster);
		return false;
	}

	m_fontRaster = RwRasterSetFromImage(m_fontRaster, font_img);
	io.Fonts->TexID = (ImTextureID*) m_fontRaster;

	RwImageDestroy(font_img);

	LOGI("Atlas width %d, height %d", width, height);
	return true;
}

void ImGuiWrapper::destroyFontTexture()
{
	if (m_fontRaster) {
		ImGuiIO& io = ImGui::GetIO();
		RwRasterDestroy(m_fontRaster);
		m_fontRaster = nullptr;
		io.Fonts->TexID = (ImTextureID) 0;
	}
}
