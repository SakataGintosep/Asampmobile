#pragma once

#include <RakNet/RakClientInterface.h>
#include <RakNet/RakNetworkFactory.h>
#include <RakNet/PacketEnumerations.h>
#include <RakNet/StringCompressor.h>
#include <RakNet/SAMP/SAMPRPC.h>

#include "../settings.h"

#include "playerpool.h"
#include "vehiclepool.h"
#include "objectpool.h"

typedef unsigned short PLAYERID;
typedef unsigned short VEHICLEID;
typedef unsigned short OBJECTID;

#define GAMESTATE_WAIT_CONNECT      1
#define GAMESTATE_CONNECTING        2
#define GAMESTATE_CONNECTED         5
#define GAMESTATE_AWAIT_JOIN        6
#define GAMESTATE_RESTARTING        11

#define MAX_PLAYERS                 1004
#define MAX_ACTORS                  1000
#define MAX_VEHICLES                2000
#define MAX_GANG_ZONES              1024
#define MAX_PICKUPS                 4096
#define MAX_OBJECTS                 1000
#define MAX_MAP_ICONS               100
#define MAX_TEXT_LABELS             2048
#define MAX_TEXT_DRAWS              2304

struct CNetGamePools {
	CPlayerPool* playerPool;
	CVehiclePool* vehiclePool;
	uintptr_t gangzonePool;
	uintptr_t pickupPool;
	CObjectPool* objectPool;
	uintptr_t textlabelPool;
	uintptr_t textdrawPool;
	uintptr_t actorPool;
	uintptr_t menuPool;
};
static_assert(sizeof(CNetGamePools) == 36, "sizeof(CNetGamePools) != 36");

class CNetGame {
public:
	CNetGame(const char* host, int port, const char* nick, const char* pass)
	{
		LOGI("CNetGame::CNetGame()");
		Memory::callFunction(SAMP_Addr(0x141948 + 1), this, host, port, nick, pass);
	}

public:
	char m_hostName[257];                // 0-257
	char m_host[257];                    // 257-514
	char m_pad[2];                       // 514-516
	int m_port;                          // 516-520
	char m_pad2[8];                      // 520-528
	RakClientInterface* m_rakClient;     // 528-532
	int unk;                             // 532-536
	int m_gameState;                     // 536-540
	int m_lastConnectAttempt;            // 540-544
	char m_pad4[400];                    // 544-944
	CNetGamePools* m_pools;              // 944-948
	int unk2;                            // 948-952
};
static_assert(sizeof(CNetGame) == 952, "sizeof(CNetGame) != 952");
