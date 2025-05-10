#pragma once

#include "game/common.h"
#include "game/scripting.h"
#include "game/rw/rw.h"
#include "game/utils.h"
#include "game/scripting.h"
#include "game/cracked_classes.h"
#include "net/netgame.h"
#include "ui/ui.h"

class SAMP {
public:
	static void initialize();
	static void initializeUI();
	static void* mainThread(void*);
	static void process();

	static bool paused() { return *(bool*) (SA_Addr(0x96B514)); }

	static CrackedUI* ui();
	static CGame* game();
	static CNetGame* netgame() { return m_netgame; }

	static bool isWidgetVisible(uintptr_t widget);
	static void setWidgetVisible(uintptr_t widget, bool visible);

	static void addDebugMessage(const char* message, ...);
	static void addInfoMessage(const char* message, ...);

private:
	static CNetGame* m_netgame;
};
