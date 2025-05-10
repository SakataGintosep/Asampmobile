#include "../main.h"
#include "../samp.h"
#include "../settings.h"
#include "../voice/Network.h"
#include "../voice/Plugin.h"

extern UI* pUI;
extern JavaVM* g_VM;

void initializeRPC_hooks();

DECL_HOOK(void, ReadSettingFile)
{
	LOGI("ReadSettingFile_hook()");
	ReadSettingFile();

	Settings::initialize();

	if (Settings::voice()) {
		Plugin::OnPluginLoad();
		Plugin::OnSampLoad();
	}

	// timestamp
	Memory::protectAddr(SAMP_Addr(0x23DC44));
	*(bool*) SAMP_Addr(0x23DC44) = Settings::timestamp();

	// chat strings
	Memory::protectAddr(SAMP_Addr(0x237518));
	*(int*) SAMP_Addr(0x237518) = Settings::chatstrings();

	// chat pos
	Memory::protectAddr(SAMP_Addr(0x238F08));
	*(ImVec2*) SAMP_Addr(0x238F08) = Settings::chatpos();

	// chat size
	/*Memory::protectAddr(SAMP_Addr(0x238F10));
	*(ImVec2*) SAMP_Addr(0x238F10) = Settings::chatsize();*/

	// chat item size
	Memory::protectAddr(SAMP_Addr(0x238F18));
	*(ImVec2*) (SAMP_Addr(0x238F18)) = ImVec2(Settings::chatsize().x, Settings::chatsize().y / 13.0f);
}

DECL_HOOK(void, InitGui)
{
	LOGI("InitGui_hook()");
	InitGui();

	SAMP::initializeUI();
}

DECL_HOOK(void, MainLoop)
{
	MainLoop();
	SAMP::process();
}

DECL_HOOK(void, TouchEvent, int type, int num, int posX, int posY)
{
	if (SAMP::paused()) {
		return TouchEvent(type, num, posX, posY);
	}

	if (pUI) {
		switch (type) {
			case 2: // push
				pUI->touchEvent(ImVec2(posX, posY), TouchType::push);
				break;

			case 3: // move
				pUI->touchEvent(ImVec2(posX, posY), TouchType::move);
				break;

			case 1: // pop
				pUI->touchEvent(ImVec2(posX, posY), TouchType::pop);
				break;
		}
	}

	TouchEvent(type, num, posX, posY);
}

// fix black color messages on some servers
DECL_HOOK(void, Chat_addChatMessage, uintptr_t a1, std::string const& message, std::string const& nick, ImColor const& nickColor)
{
	std::string color = "{" +
			utils::int_to_hex((int) (nickColor.Value.x * 255.0f)) +
			utils::int_to_hex((int) (nickColor.Value.y * 255.0f)) +
			utils::int_to_hex((int) (nickColor.Value.z * 255.0f)) + "}";

	SAMP::addDebugMessage(std::string(color + nick + "{FFFFFF}" + message).c_str());
}

// fix jetpack
DECL_HOOK(void, CTouchInterface_IsTouched, int a1, int a2, int a3)
{
	if (SAMP::game()) {
		if (SAMP::game()->findPlayerPed()->isInJetpack() && (a1 == WIDGET_ACCELERATE || a1 == WIDGET_BRAKE)) {
			return Memory::callFunction<void>(SA_Addr(0x2B0CBC + 1), a1, a2, a3); // CTouchInterface::IsTouched
		}
	}

	return CTouchInterface_IsTouched(a1, a2, a3);
}

DECL_HOOK(void, _CVehicle, CVehicle* a1, int type, float x, float y, float z, float rotation, bool siren, bool unk)
{
	_CVehicle(a1, type, x, y, z, rotation, siren, unk);
	a1->m_rotation = rotation;
}

float getDistanceFromLocalPlayerPed(CPlayerPed* playerPed, CVehicle* vehicle)
{
	if (playerPed && playerPed->m_ped && vehicle && vehicle->m_vehicle) {
		VECTOR playerPos = playerPed->m_ped->entity.mat->pos;
		VECTOR vehiclePos = vehicle->m_vehicle->entity.mat->pos;
		//LOGI("getDistanceFromLocalPlayerPed -> PlayerPos: %.1f %.1f %.1f | VehiclePos: %.1f %.1f %.1f", playerPos.X, playerPos.Y, playerPos.Z, vehiclePos.X, vehiclePos.Y, vehiclePos.Z);
		return sqrt(pow(vehiclePos.X - playerPos.X, 2) + pow(vehiclePos.Y - playerPos.Y, 2) + pow(vehiclePos.Z - playerPos.Z, 2));
	}
	return 0.0f;
}

// fix vehicles spawning on the road instead of the correct position
DECL_HOOK(void, CVehiclePool_Process, CVehiclePool* a1)
{
	CVehiclePool_Process(a1);

	if (SAMP::netgame() && SAMP::netgame()->m_gameState == GAMESTATE_CONNECTED) {
		CVehiclePool* pVehiclePool = SAMP::netgame()->m_pools->vehiclePool;
		CPlayerPed* pLocalPlayer = SAMP::game()->findPlayerPed();

		if (pVehiclePool && pLocalPlayer) {
			for (VEHICLEID x = 0; x < MAX_VEHICLES; x++) {
				CVehicle* pVehicle = pVehiclePool->getAt(x);
				if (pVehicle) {
					float distance = getDistanceFromLocalPlayerPed(pLocalPlayer, pVehicle);
					if (distance < 150.0f) {
						if (!pVehicle->isAdded()) {
							pVehicle->add();
							scriptCommand(&set_car_z_angle, pVehicle->m_gtaId, pVehicle->m_rotation);
							LOGI("Vehicle fixed (Dist: %.1f | X: %.1f | Y: %.1f | Z: %.1f | Rot: %.1f)",
									distance,
									pVehiclePool->m_position[x].X, pVehiclePool->m_position[x].Y,
									pVehiclePool->m_position[x].Z, pVehicle->m_rotation);
						}
					}
					else {
						if (pVehicle->isAdded()) {
							pVehicle->remove();
						}
					}
				}
			}
		}
	}
}

#define AUTH_BS OBFUSCATE("39FB2DEEDB49ACFB8D4EECE6953D2507988CCCF4410")

DECL_HOOK(void, CNetGame_Packet_ConnectionSucceeded, CNetGame* a1, int pkt)
{
	LOGI("CNetGame_Packet_ConnectionSucceeded_hook()");

	uint32_t pktLength = *(uint32_t*) (pkt + 12);
	uint8_t* pktData = *(uint8_t**) (pkt + 20);

	RakNet::BitStream bsSuccAuth(pktData, pktLength, true);
	PLAYERID MyPlayerID;
	unsigned int uiChallenge;

	bsSuccAuth.IgnoreBits(8);  // ID_CONNECTION_REQUEST_ACCEPTED
	bsSuccAuth.IgnoreBits(32); // binaryAddress
	bsSuccAuth.IgnoreBits(16); // port

	bsSuccAuth.Read(MyPlayerID);
	bsSuccAuth.Read(uiChallenge);

	SAMP::addDebugMessage("Connected. Joining the game...");

	const char* SAMP_VERSION = "0.3.7";
	int iVersion = 4057;
	char byteMod = 0x01;
	unsigned int uiClientChallengeResponse = uiChallenge ^ iVersion;

	// 0.3.DL
	if (Settings::sampversion() == 1) {
		SAMP_VERSION = "0.3.7-R4";
	}

	char byteAuthBSLen = (char) strlen(AUTH_BS);
	char byteNameLen = (char) strlen(Settings::nick());
	char byteClientverLen = (char) strlen(SAMP_VERSION);

	a1->m_gameState = GAMESTATE_AWAIT_JOIN;

	RakNet::BitStream bsSend;
	bsSend.Write(iVersion);
	bsSend.Write(byteMod);
	bsSend.Write(byteNameLen);
	bsSend.Write(Settings::nick(), byteNameLen);
	bsSend.Write(uiClientChallengeResponse);
	bsSend.Write(byteAuthBSLen);
	bsSend.Write(AUTH_BS, byteAuthBSLen);
	bsSend.Write(byteClientverLen);
	bsSend.Write(SAMP_VERSION, byteClientverLen);

	Network::OnRaknetRpc(RPC_ClientJoin, bsSend);

	a1->m_rakClient->RPC(&RPC_ClientJoin, &bsSend, HIGH_PRIORITY, RELIABLE, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
}

DECL_HOOK(void, CNetGame_UpdateNetwork, int a1)
{
	int pkt;
	unsigned char packetIdentifier;
	uint8_t* v3;

	while ((pkt = (*(int (__fastcall**)(uint32_t)) (**(uint32_t**) (a1 + 528) + 36))(*(uint32_t*) (a1 + 528)))) {
		v3 = *(uint8_t**) (pkt + 20);
		packetIdentifier = *v3;
		if (packetIdentifier == 40) {
			packetIdentifier = v3[5];
		}
		switch (packetIdentifier) {
			case ID_AUTH_KEY:
				// CNetGame::Packet_AuthKey
				Memory::callFunction(SAMP_Addr(0x1420BC + 1), a1, pkt);
				break;
			case ID_CONNECTION_ATTEMPT_FAILED:
				// CNetGame::Packet_ConnectAttemptFailed
				Memory::callFunction(SAMP_Addr(0x142198 + 1), a1);
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				// CNetGame::Packet_NoFreeIncomingConnections
				Memory::callFunction(SAMP_Addr(0x1421D4 + 1), a1);
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				// CNetGame::Packet_DisconnectionNotification
				Memory::callFunction(SAMP_Addr(0x142210 + 1), a1);
				break;
			case ID_CONNECTION_LOST:
				// CNetGame::Packet_ConnectionLost
				Memory::callFunction(SAMP_Addr(0x14229C + 1), a1);
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				// CNetGame::Packet_ConnectionSucceeded
				CNetGame_Packet_ConnectionSucceeded_hook((CNetGame*) a1, pkt);
				break;
			case ID_FAILED_INITIALIZE_ENCRIPTION:
				// CNetGame::Packet_FailedInitializeEncription
				Memory::callFunction(SAMP_Addr(0x142780 + 1));
				break;
			case ID_CONNECTION_BANNED:
				// CNetGame::Packet_ConnectionBanned
				Memory::callFunction(SAMP_Addr(0x1427B4 + 1), a1);
				break;
			case ID_INVALID_PASSWORD:
				// CNetGame::Packet_InvalidPassword
				Memory::callFunction(SAMP_Addr(0x142800 + 1), a1);
				break;
			case ID_VEHICLE_SYNC:
				// CNetGame::Packet_VehicleSync
				Memory::callFunction(SAMP_Addr(0x14289C + 1), a1, pkt);
				break;
			case ID_AIM_SYNC:
				// CNetGame::Packet_AimSync
				Memory::callFunction(SAMP_Addr(0x142B24 + 1), a1, pkt);
				break;
			case ID_BULLET_SYNC:
				// CNetGame::Packet_BulletSync
				Memory::callFunction(SAMP_Addr(0x142BA4 + 1), a1, pkt);
				break;
			case ID_PLAYER_SYNC:
				// CNetGame::Packet_PlayerSync
				Memory::callFunction(SAMP_Addr(0x142C2E + 1), a1, pkt);
				break;
			case ID_MARKERS_SYNC:
				// CNetGame::Packet_MarkerSync
				Memory::callFunction(SAMP_Addr(0x142E8A + 1), a1, pkt);
				break;
			case ID_UNOCCUPIED_SYNC:
				// CNetGame::Packet_UnoccupiedSync
				//Memory::callFunction(SAMP_Addr(0x142F9E + 1), a1, pkt); // causes vehicle explosion bug when enter as passenger
				break;
			case ID_TRAILER_SYNC:
				// CNetGame::Packet_TrailerSync
				Memory::callFunction(SAMP_Addr(0x143028 + 1), a1, pkt);
				break;
			case ID_PASSENGER_SYNC:
				// CNetGame::Packet_PassengerSync
				Memory::callFunction(SAMP_Addr(0x1430C8 + 1), a1, pkt);
				break;
			case ID_VOICE_SYNC:
				// voice
				Network::OnRaknetReceive(pkt);
				break;
		}

		(*(void (__fastcall**)(uint32_t, int)) (**(uint32_t**) (a1 + 528) + 40))(*(uint32_t*) (a1 + 528), pkt);
	}
}

DECL_HOOK(void, azvoice_cmd)
{
	if (pUI) {
		pUI->deathwindow()->setVisible(!pUI->deathwindow()->visible());
		SAMP::addInfoMessage("DeathWindow: %s", pUI->deathwindow()->visible() ? "visible" : "hidden");
	}
}

DECL_HOOK(void, ctrBtn_callback)
{
	if (SAMP::ui()) {
		// sub_13630C
		Memory::callFunction<void>(SAMP_Addr(0x13630C + 1), SAMP::ui()->m_playertablist); // PlayerTabList::show
	}
}

void initializeSAMPHooks()
{
	LOGI("initializeSAMPHooks()");

	// ReadSettingFile
	HOOK(SAMP_Addr(0xE3FE0), ReadSettingFile);

	// InitGui
	HOOK(SAMP_Addr(0xF0BC0), InitGui);

	// MainLoop
	HOOK(SAMP_Addr(0xF0B60), MainLoop);

	// TouchEvent
	HOOK(SAMP_Addr(0xFD040), TouchEvent);

	// Chat_addChatMessage
	HOOK(SAMP_Addr(0x12D258), Chat_addChatMessage);

	// CTouchInterface_IsTouched
	HOOK(SAMP_Addr(0xFEA2C), CTouchInterface_IsTouched);

	// CVehicle
	HOOK(SAMP_Addr(0x109470), _CVehicle);

	// CVehiclePool_Process
	HOOK(SAMP_Addr(0x150224), CVehiclePool_Process);

	// Packet_ConnectionSucceeded
	HOOK(SAMP_Addr(0x1423A0), CNetGame_Packet_ConnectionSucceeded);

	// CNetGame_UpdateNetwork
	HOOK(SAMP_Addr(0x141DFC), CNetGame_UpdateNetwork);

	// azvoice_cmd
	HOOK(SAMP_Addr(0x12CE78), azvoice_cmd);

	// ctrBtn_callback
	HOOK(SAMP_Addr(0x12C9E0), ctrBtn_callback);

	initializeRPC_hooks();
}
