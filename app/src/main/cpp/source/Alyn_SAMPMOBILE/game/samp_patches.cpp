#include "../main.h"
#include "../samp.h"
#include "../settings.h"

void initializeSAMPPatches()
{
	LOGI("initializeSAMPPatches()");

	// sub_16447C
	Memory::ret(SAMP_Addr(0x16447C)); // LoadBassLibrary

	// use samp texdb only
	Memory::memWrite(SAMP_Addr(0xFB9C2), "\x04\x2E", 2); // before 38 2E (for 14 texdbs)
	Memory::memWrite(SAMP_Addr(0xFE210), "\x01\x2D", 2); // before 0E 2D (for 14 texdbs)

	// increase vehicle class size to add m_rotation
	Memory::memWrite(SAMP_Addr(0xF9F2C), "\x54\x20", 2);
	Memory::memWrite(SAMP_Addr(0x107A36), "\x54\x20", 2);

	// sub_F4D7A
	Memory::ret(SAMP_Addr(0xF4D7A)); // custom map icon

	// sub_F3D06 (makes the game freeze)
	Memory::ret(SAMP_Addr(0xF3D06)); // AddInCarAnim

	// sub_F3F4A (makes the game freeze)
	Memory::ret(SAMP_Addr(0xF3F4A)); // CustomEnterAnim

	// fix vehicles health doesnt go below 250.0
	Memory::nop(SAMP_Addr(0x13F270), 6);

	// delete custom menu when holding map
	Memory::nop(SAMP_Addr(0x141DCA), 10);
	Memory::nop(SAMP_Addr(0x141DE0), 2);

	// fix veh nitro button
	Memory::nop(SAMP_Addr(0x10379E), 2);

	// CLocalPlayer::SendUnoccupied
	Memory::ret(SAMP_Addr(0x141580)); // fix vehicle explosion bug when enter as passenger

	// fix garages not opening
	// Memory::nop(SAMP_Addr(0x100752), 2); // cause crash (todo find other way)

	// .rodata:0008C76B	00000014	C	/SAMP/settings.json
	Memory::memWrite(SAMP_Addr(0x08C76B), "/SAMP/setting_.json", 20);

	// .rodata:0009098A	00000009	C	arz_logo
	Memory::memWrite(SAMP_Addr(0x9098A), "mylogo", 7);

	// .rodata:00084CAB	00000008	C	seatcar
	Memory::memWrite(SAMP_Addr(0x84CAB), "gtexture", 9); // fix g button texture

	// .rodata:0008C77F	00000008	C	gundoor
	Memory::memWrite(SAMP_Addr(0x8C77F), "shoot", 6); // fix driveby button texture

	// .rodata:0008480E	0000000B	C	icon_micro
	Memory::memWrite(SAMP_Addr(0x8480E), "mylogo", 7);

	// .rodata:0008C9FE	0000000D	C	express_menu
	Memory::memWrite(SAMP_Addr(0x8C9FE), "mylogo", 7);

	// .rodata:0008ED12	00000008	C	menu_bg
	Memory::memWrite(SAMP_Addr(0x8ED12), "mylogo", 7);

	// .rodata:000898BE	0000000B	C	menu_atlas
	Memory::memWrite(SAMP_Addr(0x898BE), "mylogo", 7);

	// .rodata:00088326	00000006	C	voice
	Memory::memWrite(SAMP_Addr(0x88326), "togdw", 6); // replaced /voice with /togdw command

	// .rodata:000874BD 43 54 52 00 aCtr            DCB "CTR"
	Memory::memWrite(SAMP_Addr(0x874BD), "TAB", 4); // replaced CTR with TAB button

	// byte_8F469
	Memory::memWrite(SAMP_Addr(0x8F469), "Connecting to server | (attemps: %d)", 37);

	// byte_8F4B9
	Memory::memWrite(SAMP_Addr(0x8F4B9), "Connected to %.64s", 19);

	// byte_901C3
	Memory::memWrite(SAMP_Addr(0x901C3), "The server didn't respond. Retrying...", 39);

	// byte_87BBC
	Memory::memWrite(SAMP_Addr(0x87BBC), "The server is full. Retrying...", 32);

	// byte_9020A
	Memory::memWrite(SAMP_Addr(0x9020A), "Server closed the connection.", 30);

	// byte_8A5D2
	Memory::memWrite(SAMP_Addr(0x8A5D2), "Lost connection to the server. Reconnecting...", 47);

	// byte_87545
	Memory::memWrite(SAMP_Addr(0x87545), "You are banned from this server.", 33);

	// byte_87C00
	Memory::memWrite(SAMP_Addr(0x87C00), "Wrong server password.", 23);

	// byte_883E8
	Memory::memWrite(SAMP_Addr(0x883E8), "The server is restarting...", 28);

	// bNetworkInited
	Memory::protectAddr(SAMP_Addr(0x23DF1A));
	*(bool*) (SAMP_Addr(0x23DF1A)) = true;
}
