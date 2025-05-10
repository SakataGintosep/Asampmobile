#pragma once

#include "../game/cracked_classes.h"

class CLocalPlayer {
public:
	uint32_t getPlayerColorAsRGBA()
	{
		// CLocalPlayer::GetPlayerColorAsRGBA
		return Memory::callFunction<uint32_t>(SAMP_Addr(0x1413FC + 1), this);
	}

public:
	char m_pad[28];           // 0-28
	CPlayerPed* m_playerPed;  // 28-32
	char m_pad2[456];         // 32-488
};
static_assert(sizeof(CLocalPlayer) == 488, "sizeof(CLocalPlayer) != 488");
