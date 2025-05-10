#pragma once

#include "../game/cracked_classes.h"

class CRemotePlayer {
public:
	bool isActive()
	{
		return (m_playerPed && m_state != 0);
	}

	uint32_t getPlayerColor()
	{
		// CRemotePlayer::GetPlayerColor
		return Memory::callFunction<uint32_t>(SAMP_Addr(0x14A402 + 1), this);
	}

public:
	uint8_t m_pad[25];       // 0-25
	uint8_t m_state;         // 25-26
	uint8_t m_pad2[270];     // 26-296
	CPlayerPed* m_playerPed; // 296-300
	uint8_t m_pad3[20];      // 300-320
};
static_assert(sizeof(CRemotePlayer) == 320, "sizeof(CRemotePlayer) != 320");
