#include "main.h"
#include "samp.h"
#include "settings.h"
#include"game/hooks_patches.h"
#include"game/audiostream.h"
#include "voice/Plugin.h"
#include "voice/Network.h"

UI* pUI = nullptr;
AudioStream* pAudioStream = nullptr;

CNetGame* SAMP::m_netgame = nullptr;

bool g_gameInited = false;
bool g_netgameInited = false;

extern bool g_bAudioStreamPlaying;

void SAMP::initialize()
{
	LOGI("SAMP::initialize()");

	initializeRenderWare();
	loadBassLibrary();

	initializeSAPatches();
	initializeSAHooks();

	initializeSAMPPatches();
	initializeSAMPHooks();
}

void SAMP::initializeUI()
{
	const char* gameStorage = (const char*) (SA_Addr(0x6D687C));
	std::string font_path = std::string(gameStorage) + "fonts/arial_bold.ttf";
	pUI = new UI(ImVec2(RsGlobal->maximumWidth, RsGlobal->maximumHeight), font_path);
	pUI->initialize();
	pUI->performLayout();
}

void* SAMP::mainThread(void*)
{
	while (*(uint32_t*) (SA_Addr(0xA987C8)) != 7)
		usleep(500);

	const char* gameStorage = (const char*) (SA_Addr(0x6D687C));
	LOGI("Game storage: %s", gameStorage);

	initializeScripting();

	pthread_exit(nullptr);
}

void SAMP::process()
{
	if (!g_gameInited) {
		LOGI("Initializing game...");

		LogVoice("[dbg:samp:load] : module loading...");

		for (const auto& loadCallback : SampVoice::loadCallbacks) {
			if (loadCallback != nullptr) {
				loadCallback();
			}
		}

		SampVoice::loadStatus = true;

		LogVoice("[dbg:samp:load] : module loaded");

		g_gameInited = true;
	}

	if (!g_netgameInited) {
		LOGI("Initializing netgame...");
		m_netgame = new CNetGame(Settings::ip(), Settings::port(), Settings::nick(), Settings::pass());

		// pNetGame
		Memory::protectAddr(SAMP_Addr(0x23DEF4));
		*(CNetGame**) (SAMP_Addr(0x23DEF4)) = m_netgame;

		Network::OnRaknetConnect(Settings::ip(), Settings::port());

		pAudioStream = new AudioStream();
		pAudioStream->Initialize();

		addDebugMessage("{FFFFFF}Alyn {0b5394}SA-MP Mobile {FFFFFF}Started");
		addDebugMessage("{FFFFFF}Client commands: {0b5394}/q /dl /odl /togdw /headmove /timestamp /pagesize<5-20> /fontsize<0.1-2.0>");
		addDebugMessage(" ");

		g_netgameInited = true;
	}

	if (pUI) {
		pUI->render();
	}

	if (m_netgame) {
		if (pAudioStream) {
			pAudioStream->Process();
		}

		if (m_netgame->m_gameState != GAMESTATE_CONNECTED) {
			if (pAudioStream && g_bAudioStreamPlaying) {
				pAudioStream->Stop(true);
			}
		}
	}
}

CrackedUI* SAMP::ui()
{
	CrackedUI* uiPtr = *(CrackedUI**) (SAMP_Addr(0x23DEEC));

	if (!uiPtr) {
		LOGI("UI pointer is null");
		return nullptr;
	}

	return uiPtr;
}

CGame* SAMP::game()
{
	CGame* gamePtr = *(CGame**) (SAMP_Addr(0x23DEF0));

	if (!gamePtr) {
		LOGI("Game pointer is null");
		return nullptr;
	}

	return gamePtr;
}

bool SAMP::isWidgetVisible(uintptr_t widget)
{
	if (!ui()) {
		LOGI("SAMP::isWidgetVisible: UI pointer is null");
		return false;
	}

	return *(bool*) (widget + 80);
}

void SAMP::setWidgetVisible(uintptr_t widget, bool visible)
{
	if (!ui()) {
		LOGI("SAMP::setWidgetVisible: UI pointer is null");
		return;
	}

	*(bool*) (widget + 80) = visible;
}

void SAMP::addDebugMessage(const char* message, ...)
{
	if (!ui()) {
		LOGI("SAMP::addDebugMessage: UI pointer is null");
		return;
	}

	char tmp_buf[512];

	va_list args;
	va_start(args, message);
	vsprintf(tmp_buf, message, args);
	va_end(args);

	// sub_12D5E8
	Memory::callFunction(SAMP_Addr(0x12D5E8 + 1), ui()->m_chat, tmp_buf); // Chat::addDebugMessageU8
}

void SAMP::addInfoMessage(const char* message, ...)
{
	if (!ui()) {
		LOGI("SAMP::addInfoMessage: UI pointer is null");
		return;
	}

	char tmp_buf[512];

	va_list args;
	va_start(args, message);
	vsprintf(tmp_buf, message, args);
	va_end(args);

	// sub_12D490
	Memory::callFunction(SAMP_Addr(0x12D490 + 1), ui()->m_chat, tmp_buf); // Chat::addInfoMessage
}
