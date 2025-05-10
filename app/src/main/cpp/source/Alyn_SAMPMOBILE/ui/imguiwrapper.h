#pragma once

#include "imguirenderer.h"
#include "../game/rw/rw.h"

class ImGuiWrapper {
public:
	ImGuiWrapper(const ImVec2& display_size, const std::string& font_path);

	virtual bool initialize();
	virtual void render();
	virtual void shutdown();

	const ImVec2& displaySize() { return m_displaySize; }
	ImGuiRenderer* renderer() const { return m_renderer; }

	ImFont* weapFont() const { return m_weapFont; }

	void renderDrawData(ImDrawData* draw_data);

protected:
	virtual ~ImGuiWrapper();
	virtual void drawList() = 0;

private:
	void setupRenderState();
	void checkVertexBuffer(ImDrawData* draw_data);
	bool createFontTexture();
	void destroyFontTexture();

private:
	ImVec2 m_displaySize;
	std::string m_fontPath;

	ImFont* m_weapFont;

	ImGuiRenderer* m_renderer;
	RwRaster* m_fontRaster;

	RwIm2DVertex* m_vertexBuffer;
	int m_vertexBufferSize;
};
