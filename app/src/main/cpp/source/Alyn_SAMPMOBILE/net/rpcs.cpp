#include "../main.h"
#include "../samp.h"
#include "../game/audiostream.h"

#define REJECT_REASON_BAD_VERSION 1
#define REJECT_REASON_BAD_NICKNAME 2
#define REJECT_REASON_BAD_MOD 3
#define REJECT_REASON_BAD_PLAYERID 4

extern UI* pUI;
extern AudioStream* pAudioStream;

extern bool g_disableVehicleCollisions;

DECL_HOOK(void, InitGame, RPCParameters* rpcParams)
{
	LOGI("InitGame_hook()");
	InitGame(rpcParams);

	auto Data = reinterpret_cast<unsigned char*>(rpcParams->input);
	unsigned int iBitLength = rpcParams->numberOfBitsOfData;
	RakNet::BitStream bsData(Data, (iBitLength / 8) + 1, false);

	bool bZoneNames, bUseCJWalk;

	bsData.Read(bZoneNames);
	bsData.Read(bUseCJWalk);

	LOGI("bZoneNames: %d", bZoneNames);
	LOGI("bUseCJWalk: %d", bUseCJWalk);

	if (bUseCJWalk) {
		Memory::memWrite(SA_Addr(0x4C5EFA), "\xCA\xF8\xE0\x04", 4);
	}
	else {
		Memory::nop(SA_Addr(0x4C5EFA), 2);
	}
}

DECL_HOOK(void, DialogBox, RPCParameters* rpcParams)
{
	LOGI("DialogBox_hook()");
	DialogBox(rpcParams);

	auto Data = reinterpret_cast<unsigned char*>(rpcParams->input);
	unsigned int iBitLength = rpcParams->numberOfBitsOfData;
	RakNet::BitStream bsData(Data, (iBitLength / 8) + 1, false);

	int16_t wDialogID;
	uint8_t byteDialogStyle;
	uint8_t byteLength;
	char szTitle[256 + 1];
	char szButton1[256 + 1];
	char szButton2[256 + 1];
	char szMessage[4096 + 1];
	memset(szTitle, 0, sizeof(szTitle));
	memset(szButton1, 0, sizeof(szButton1));
	memset(szButton2, 0, sizeof(szButton2));
	memset(szMessage, 0, sizeof(szMessage));

	// id & style
	bsData.Read(wDialogID);
	bsData.Read(byteDialogStyle);

	// title
	bsData.Read(byteLength);
	bsData.Read(szTitle, byteLength);

	// button1
	bsData.Read(byteLength);
	bsData.Read(szButton1, byteLength);

	// button2
	bsData.Read(byteLength);
	bsData.Read(szButton2, byteLength);

	// message
	//stringCompressor->DecodeString(szMessage, 4096, &bsData); // crashes

	LOGI("wDialogID %d", wDialogID);
	LOGI("byteDialogStyle %d", (int) byteDialogStyle);
	LOGI("szTitle %s", szTitle);
	LOGI("szButton1 %s", szButton1);
	LOGI("szButton2 %s", szButton2);
	LOGI("szMessage %s", szMessage);

	if (wDialogID == 65535 || wDialogID < 0)
	{
		LOGI("Fix Blank Dialog");

		if (SAMP::isWidgetVisible(SAMP::ui()->m_dialog)) {
			SAMP::setWidgetVisible(SAMP::ui()->m_dialog, false);
		}
	}
}

DECL_HOOK(void, ConnectionRejected, RPCParameters* rpcParams)
{
	LOGI("ConnectionRejected_hook()");

	auto Data = reinterpret_cast<unsigned char*>(rpcParams->input);
	unsigned int iBitLength = rpcParams->numberOfBitsOfData;
	RakNet::BitStream bsData(Data, (iBitLength / 8) + 1, false);

	uint8_t byteRejectReason;
	bsData.Read(byteRejectReason);

	switch (byteRejectReason) {
		case REJECT_REASON_BAD_VERSION: SAMP::addInfoMessage("CONNECTION REJECTED: Incorrect Version.");
			break;

		case REJECT_REASON_BAD_NICKNAME: SAMP::addInfoMessage("CONNECTION REJECTED: Unacceptable NickName");
			SAMP::addInfoMessage("Please choose another nick between and 3-20 characters");
			SAMP::addInfoMessage("Please use only a-z, A-Z, 0-9");
			SAMP::addInfoMessage("Use /quit to exit or press ESC and select Quit Game");
			break;

		case REJECT_REASON_BAD_MOD: SAMP::addInfoMessage("CONNECTION REJECTED: Bad mod version.");
			break;
		case REJECT_REASON_BAD_PLAYERID: SAMP::addInfoMessage("CONNECTION REJECTED: Unable to allocate a player slot.");
			break;
	}

	SAMP::netgame()->m_rakClient->Disconnect(500);
}

void ScrDisableRemoteVehicleCollisions(RPCParameters* rpcParams)
{
	LOGI("RPC: ScrDisableRemoteVehicleCollisions");

	auto Data = reinterpret_cast<unsigned char*>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	RakNet::BitStream bsData(Data, (iBitLength / 8) + 1, false);

	bool bDisable;
	bsData.Read(bDisable);

	g_disableVehicleCollisions = bDisable;
}

void ScrPlayAudioStream(RPCParameters* rpcParams)
{
	LOGI("RPC: ScrPlayAudioStream");

	auto Data = reinterpret_cast<unsigned char*>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	RakNet::BitStream bsData(Data, (iBitLength / 8) + 1, false);

	uint8_t byteUrlLen;
	char szUrl[256];
	float fX, fY, fZ;
	float fRadius;
	uint8_t byteUsePos;
	memset(szUrl, 0, sizeof(szUrl));

	bsData.Read(byteUrlLen);
	bsData.Read(szUrl, byteUrlLen);
	bsData.Read(fX);
	bsData.Read(fY);
	bsData.Read(fZ);
	bsData.Read(fRadius);
	bsData.Read(byteUsePos);

	SAMP::addInfoMessage("[Audio Stream]: %s", szUrl);

	if (pAudioStream) {
		pAudioStream->Play(szUrl, fX, fY, fZ, fRadius, byteUsePos);
	}
}

void ScrStopAudioStream(RPCParameters* rpcParams)
{
	LOGI("RPC: ScrStopAudioStream");

	if (pAudioStream) {
		pAudioStream->Stop(false);
	}
}

void ScrDeathMessage(RPCParameters* rpcParams)
{
	LOGI("RPC: ScrDeathMessage");

	auto Data = reinterpret_cast<unsigned char*>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	RakNet::BitStream bsData(Data, (iBitLength / 8) + 1, false);

	PLAYERID playerId, killerId;
	uint8_t reason;

	bsData.Read(killerId);
	bsData.Read(playerId);
	bsData.Read(reason);

	std::string killername, playername;
	unsigned int killercolor, playercolor;
	killername.clear();
	playername.clear();

	if (!SAMP::netgame()) return;

	CPlayerPool* pPlayerPool = SAMP::netgame()->m_pools->playerPool;
	if (pPlayerPool) {
		if (pPlayerPool->m_localPlayerID == playerId) {
			playername = pPlayerPool->m_localPlayerName;
			playercolor = pPlayerPool->m_localPlayer->getPlayerColorAsRGBA();
		}
		else {
			if (pPlayerPool->getSlotState(playerId)) {
				playername = pPlayerPool->getPlayerName(playerId);
				playercolor = pPlayerPool->getAt(playerId)->getPlayerColor();
			}
		}

		if (pPlayerPool->m_localPlayerID == killerId) {
			killername = pPlayerPool->m_localPlayerName;
			killercolor = pPlayerPool->m_localPlayer->getPlayerColorAsRGBA();
		}
		else {
			if (pPlayerPool->getSlotState(killerId)) {
				killername = pPlayerPool->getPlayerName(killerId);
				killercolor = pPlayerPool->getAt(killerId)->getPlayerColor();
			}
		}
	}

	pUI->deathwindow()->record(playername.c_str(), UI::fixcolor(playercolor), killername.c_str(), killercolor, reason);
}

DECL_HOOK(void, RegisterRPCs, RakClientInterface* rakClient)
{
	LOGI("RegisterRPCs_hook()");
	RegisterRPCs(rakClient);

	rakClient->RegisterAsRemoteProcedureCall(&RPC_ScrDisableVehicleCollisions, ScrDisableRemoteVehicleCollisions);
	rakClient->RegisterAsRemoteProcedureCall(&RPC_ScrPlayAudioStream, ScrPlayAudioStream);
	rakClient->RegisterAsRemoteProcedureCall(&RPC_ScrStopAudioStream, ScrStopAudioStream);
	rakClient->RegisterAsRemoteProcedureCall(&RPC_ScrDeathMessage, ScrDeathMessage);
}

DECL_HOOK(void, UnregisterRPCs, RakClientInterface* rakClient)
{
	LOGI("UnregisterRPCs_hook()");
	UnregisterRPCs(rakClient);

	rakClient->UnregisterAsRemoteProcedureCall(&RPC_ScrDisableVehicleCollisions);
	rakClient->UnregisterAsRemoteProcedureCall(&RPC_ScrPlayAudioStream);
	rakClient->UnregisterAsRemoteProcedureCall(&RPC_ScrStopAudioStream);
	rakClient->UnregisterAsRemoteProcedureCall(&RPC_ScrDeathMessage);
}

void initializeRPC_hooks()
{
	LOGI("initializeRPC_hooks()");

	// InitGame
	HOOK(SAMP_Addr(0x144C9C), InitGame);

	// DialogBox
	HOOK(SAMP_Addr(0x145AF4), DialogBox);

	// ConnectionRejected
	HOOK(SAMP_Addr(0x146458), ConnectionRejected);

	// RegisterRPCs
	HOOK(SAMP_Addr(0x14780C), RegisterRPCs);

	// UnregisterRPCs
	//HOOK(SAMP_Addr(0x0), UnregisterRPCs);
}
