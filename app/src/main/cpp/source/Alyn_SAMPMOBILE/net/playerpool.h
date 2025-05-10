#pragma once

#include "player.h"
#include "localplayer.h"

typedef unsigned short PLAYERID;

#define MAX_PLAYERS 1004
#define INVALID_PLAYER_ID 0xFFFF

#define MAX_PLAYER_NAME 24

class CPlayerPool {
public:
	CPlayerPool() = default;
	~CPlayerPool() = default;

	CRemotePlayer* getAt(PLAYERID playerId)
	{
		if (playerId > MAX_PLAYERS || !m_slotState[playerId] || m_players[playerId] == nullptr) {
			return nullptr;
		}
		return m_players[playerId]->m_remotePlayer;
	}

	bool getSlotState(PLAYERID playerId)
	{
		if (playerId > MAX_PLAYERS) {
			return false;
		}
		return m_slotState[playerId];
	}

	char* getPlayerName(PLAYERID playerId)
	{
		if (playerId == m_localPlayerID) {
			return m_localPlayerName;
		}
		else if (playerId <= MAX_PLAYERS && m_players[playerId] != nullptr) {
			return m_players[playerId]->m_playerName;
		}
		return nullptr;
	}

	int getTotalPlayers()
	{
		int iTotal = 1;

		for (int i = 0; i < MAX_PLAYERS; i++) {
			if (getSlotState(i)) {
				iTotal++;
			}
		}

		return iTotal;
	}

public:
	int m_lastPlayerID;                          // 0-4
	CPlayer* m_players[MAX_PLAYERS];             // 4-4020
	bool m_slotState[MAX_PLAYERS];               // 4020-5024
	PLAYERID m_localPlayerID;                    // 5024-5026
	char m_localPlayerName[MAX_PLAYER_NAME + 1]; // 5026-5051
	char m_pad[1];                               // 5051-5052
	CLocalPlayer* m_localPlayer;                 // 5052-5056
	int m_localPlayerPing;                       // 5056-5060
	int m_localPlayerScore;                      // 5060-5064
	char m_pad2[1016];                           // 5064-6080
};
static_assert(sizeof(CPlayerPool) == 6080, "sizeof(CPlayerPool) != 6080");
