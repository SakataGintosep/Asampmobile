#pragma once

#define MAX_PLAYER_NAME 24

#include "remoteplayer.h"

#pragma pack(push, 1)
class CPlayer {
public:
	CRemotePlayer* m_remotePlayer;          // 0-4
	char m_playerName[MAX_PLAYER_NAME + 1]; // 4-29
	bool m_npc;                             // 29-30
	int m_score;                            // 30-34
	int m_ping;                             // 34-38
	uint16_t unk;                           // 38-40
};
static_assert(sizeof(CPlayer) == 40, "sizeof(CPlayer) != 40");
#pragma pack(pop)
