#pragma once

#include <stdint.h>

#define PADDING(x, y) uint8_t x[y]

#define IN_VEHICLE(x) ((x->dwStateFlags & 0x100) >> 8)
#define IS_CROUCHING(x) ((x->dwStateFlags >> 26) & 1)

enum ePadKeys {
	// KEY_ 						OnFoot 				InCar
	KEY_ACTION = 0,        // TAB 			| 	ALT GR/LCTRL/NUM0	|	1 //4-5
	KEY_CROUCH,            // C 			|	H / CAPSLOCK		| 	2 //5-6
	KEY_FIRE,              // LCTRL/LMB		|	LALT				| 	4 //6-7
	KEY_SPRINT,            // SPACE			| 	W 					|	8 //7-8
	KEY_SECONDARY_ATTACK,  // ENTER			| 	ENTER 				|	16 //8-9
	KEY_JUMP,              // LSHIFT		|	S 					| 	32 //9-10
	KEY_LOOK_RIGHT,        // 				| 	E 					| 	64 //10-11
	KEY_HANDBRAKE,         // RMB			|	SPACE 				| 	128 //11-12
	KEY_LOOK_LEFT,         // 				|	Q 					| 	256 //12-13
	KEY_SUBMISSION,        // NUM1/MMB		| 	2/NUMPAD+ 			| 	512 //13-14
	KEY_WALK,              // LALT			| 	 					| 	1024 //14-15
	KEY_ANALOG_UP,         // NUM8			| 	NUM8				| 	2048 //15-16
	KEY_ANALOG_DOWN,       // NUM2 			| 	NUM2				|	4096 //16-17
	KEY_ANALOG_LEFT,       // NUM4			| 	NUM4				|	8192 //17-18
	KEY_ANALOG_RIGHT,      // NUM6			| 	NUM6				| 	16384 //18-19
	KEY_YES,               // Y 			| 	Y 					| 	65536 //19-20
	KEY_NO,                // N 			| 	N 					| 	131072 //20-21
	KEY_CTRL_BACK,         // H 			|   H 					|	262144 //21-22
	SIZE
};

#pragma pack(push, 1)
typedef struct {
	uint16_t wKeyLR;//0-2
	uint16_t wKeyUD;//2-4
	bool bKeys[ePadKeys::SIZE];//4-22
	bool bIgnoreJump;//22-23
} PAD_KEYS;
static_assert(sizeof(PAD_KEYS) == 23);
#pragma pack(pop)

enum SAWidgets {
	WIDGET_ENTER_CAR = 0x0,
	WIDGET_ATTACK = 0x1,
	WIDGET_ACCELERATE = 0x2,
	WIDGET_BRAKE = 0x3,
	WIDGET_HANDBRAKE = 0x4,
	WIDGET_VEHICLE_STEER_LEFT = 0x5,
	WIDGET_VEHICLE_STEER_RIGHT = 0x6,
	WIDGET_HORN = 0x7,
	WIDGET_PHONE = 0x8,
	WIDGET_GIFT = 0x9,
	WIDGET_KISS = 0xA,
	WIDGET_CAR_SHOOT = 0xB,
	WIDGET_HYDRAULICS = 0xC,
	WIDGET_AUTO_HYDRAULICS = 0xD,
	WIDGET_DROP_CRANE = 0xE,
	WIDGET_DROP_ITEM = 0xF,
	WIDGET_VEHICLE_SHOOT_LEFT = 0x10,
	WIDGET_VEHICLE_SHOOT_RIGHT = 0x11,
	WIDGET_CAM_TOGGLE = 0x12,
	WIDGET_ENTER_TARGETING = 0x13,
	WIDGET_EXIT_TARGETING = 0x14,
	WIDGET_VC_SHOOT = 0x15,
	WIDGET_VC_SHOOT_ALT = 0x16,
	WIDGET_AIR_GUN = 0x17,
	WIDGET_ROCKET = 0x18,
	WIDGET_VEHICLE_BOMB = 0x19,
	WIDGET_VEHICLE_TURRET_LEFT = 0x1A,
	WIDGET_VEHICLE_TURRET_RIGHT = 0x1B,
	WIDGET_PURCHASE = 0x1C,
	WIDGET_SWAP_WEAPONS = 0x1D,
	WIDGET_NITRO = 0x1E,
	WIDGET_BUTTON_SPRINT = 0x1F,
	WIDGET_BUTTON_CROUCH = 0x20,
	WIDGET_BUTTON_DIVE = 0x21,
	WIDGET_BUTTON_SWIM = 0x22,
	WIDGET_POOL_ENGLISH = 0x23,
	WIDGET_POOL_BALL_IN_HAND = 0x24,
	WIDGET_ARCADE_BUTTON = 0x25,
	WIDGET_ARCADE_POWER_OFF = 0x26,
	WIDGET_ARCADE_RESET = 0x27,
	WIDGET_ARCADE_JOYSTICK = 0x28,
	WIDGET_SKIP_CUTSCENE = 0x29,
	WIDGET_TRAIN_NEXT_STATION = 0x2A,
	WIDGET_TAP_METER_BUTTON = 0x2B,
	WIDGET_CRANE_UP = 0x2C,
	WIDGET_CRANE_DOWN = 0x2D,
	WIDGET_HELICOPTER_MAGNET_UP = 0x2E,
	WIDGET_HELICOPTER_MAGNET_DOWN = 0x2F,
	WIDGET_GYM_EASIER = 0x30,
	WIDGET_GYM_HARDER = 0x31,
	WIDGET_ZOOM_IN = 0x32,
	WIDGET_ZOOM_OUT = 0x33,
	WIDGET_RACE_LEFT = 0x34,
	WIDGET_RACE_RIGHT = 0x35,
	WIDGET_STORE_ACCEPT = 0x36,
	WIDGET_STORE_CANCEL = 0x37,
	WIDGET_STORE_BACK = 0x38,
	WIDGET_STORE_BUY = 0x39,
	WIDGET_ROULETTE_UP = 0x3A,
	WIDGET_ROULETTE_DOWN = 0x3B,
	WIDGET_ROULETTE_LEFT = 0x3C,
	WIDGET_ROULETTE_RIGHT = 0x3D,
	WIDGET_PLACE_BET = 0x3E,
	WIDGET_REMOVE_BET = 0x3F,
	WIDGET_STEERING_SELECTION = 0x40,
	WIDGET_TOUCH_LAYOUT_SELECTION = 0x41,
	WIDGET_GRAB = 0x42,
	WIDGET_MAGNET = 0x43,
	WIDGET_STINGER = 0x44,
	WIDGET_MISSION_START = 0x45,
	WIDGET_MISSION_START_VIGILANTE = 0x46,
	WIDGET_MISSION_CANCEL = 0x47,
	WIDGET_MISSION_CANCEL_VIGILANTE = 0x48,
	WIDGET_MINIGAME_START = 0x49,
	WIDGET_MINIGAME_CANCEL = 0x4A,
	WIDGET_SCHOOL_EXIT = 0x4B,
	WIDGET_SCHOOL_LEFT = 0x4C,
	WIDGET_SCHOOL_RIGHT = 0x4D,
	WIDGET_SCHOOL_START = 0x4E,
	WIDGET_SCHOOL_CONTINUE = 0x4F,
	WIDGET_SCHOOL_TITLE_BACKGROUND = 0x50,
	WIDGET_SCHOOL_AWARD = 0x51,
	WIDGET_SCHOOL_RESULTS = 0x52,
	WIDGET_SCHOOL_FRAME_ONE = 0x53,
	WIDGET_SCHOOL_FRAME_TWO = 0x54,
	WIDGET_SCHOOL_FRAME_THREE = 0x55,
	WIDGET_SCHOOL_FRAME_FOUR = 0x56,
	WIDGET_BASKETBALL_SHOOT = 0x57,
	WIDGET_BASKETBALL_TRICK = 0x58,
	WIDGET_GAMBLING_QUIT = 0x59,
	WIDGET_GAMBLING_CONTINUE = 0x5A,
	WIDGET_BLACK_JACK_SPLIT = 0x5B,
	WIDGET_BLACK_JACK_DOUBLE = 0x5C,
	WIDGET_BLACK_JACK_HIT = 0x5D,
	WIDGET_BLACK_JACK_STAND = 0x5E,
	WIDGET_BLACK_JACK_REGION = 0x5F,
	WIDGET_MENU_SLIDER = 0x60,
	WIDGET_RHYTHM_UP = 0x61,
	WIDGET_RHYTHM_DOWN = 0x62,
	WIDGET_RHYTHM_LEFT = 0x63,
	WIDGET_RHYTHM_RIGHT = 0x64,
	WIDGET_RHYTHM_SELECT = 0x65,
	WIDGET_RHYTHM_EXIT = 0x66,
	WIDGET_PLAYER_SCORE = 0x67,
	WIDGET_OPPOSITION_SCORE = 0x68,
	WIDGET_GANG_REGION = 0x69,
	WIDGET_GANG_RECRUIT = 0x6A,
	WIDGET_GANG_IGNORE = 0x6B,
	WIDGET_GANG_FOLLOW = 0x6C,
	WIDGET_GANG_HOLD_POSITION = 0x6D,
	WIDGET_POSITIVE = 0x6E,
	WIDGET_NEGATIVE = 0x6F,
	WIDGET_COLOR_PICKER = 0x70,
	WIDGET_VEHICLE_SLIDER_LEFT_BUTTON = 0x71,
	WIDGET_VEHICLE_SLIDER_RIGHT_BUTTON = 0x72,
	WIDGET_WOF_FIRST_BET = 0x73,
	WIDGET_WOF_SECOND_BET = 0x74,
	WIDGET_WOF_THIRD_BET = 0x75,
	WIDGET_WOF_FOURTH_BET = 0x76,
	WIDGET_WOF_FIFTH_BET = 0x77,
	WIDGET_WOF_SIXTH_BET = 0x78,
	WIDGET_VIDEO_POKER_ADD_COIN = 0x79,
	WIDGET_VIDEO_POKER_REMOVE_COIN = 0x7A,
	WIDGET_VIDEO_POKER_DEAL = 0x7B,
	WIDGET_VIDEO_POKER_HOLD_FIRST_CARD = 0x7C,
	WIDGET_VIDEO_POKER_HOLD_SECOND_CARD = 0x7D,
	WIDGET_VIDEO_POKER_HOLD_THIRD_CARD = 0x7E,
	WIDGET_VIDEO_POKER_HOLD_FOURTH_CARD = 0x7F,
	WIDGET_VIDEO_POKER_HOLD_FIFTH_CARD = 0x80,
	WIDGET_VIDEO_POKER_HOLD_FIRST_CARD_REGION = 0x81,
	WIDGET_VIDEO_POKER_HOLD_SECOND_CARD_REGION = 0x82,
	WIDGET_VIDEO_POKER_HOLD_THIRD_CARD_REGION = 0x83,
	WIDGET_VIDEO_POKER_HOLD_FOURTH_CARD_REGION = 0x84,
	WIDGET_VIDEO_POKER_HOLD_FIFTH_CARD_REGION = 0x85,
	WIDGET_VIDEO_POKER_EXIT = 0x86,
	WIDGET_ITB_FIRST_HORSE = 0x87,
	WIDGET_ITB_SECOND_HORSE = 0x88,
	WIDGET_ITB_THIRD_HORSE = 0x89,
	WIDGET_ITB_FOURTH_HORSE = 0x8A,
	WIDGET_ITB_FIFTH_HORSE = 0x8B,
	WIDGET_ITB_FIRST_BET = 0x8C,
	WIDGET_ITB_SECOND_BET = 0x8D,
	WIDGET_ITB_THIRD_BET = 0x8E,
	WIDGET_ITB_FOURTH_BET = 0x8F,
	WIDGET_ITB_FIFTH_BET = 0x90,
	WIDGET_ITB_SIXTH_BET = 0x91,
	WIDGET_ITB_SEVENTH_BET = 0x92,
	WIDGET_ITB_EIGHTH_BET = 0x93,
	WIDGET_ITB_PLACE_BET = 0x94,
	WIDGET_ITB_EXIT = 0x95,
	WIDGET_MAP = 0x96,
	WIDGET_WEAPON_SLOT_CHEATS = 0x97,
	WIDGET_CAR_CHEATS = 0x98,
	WIDGET_MISSION_SKIP = 0x99,
	WIDGET_CHEATS = 0x9A,
	WIDGET_CHEAT_MENU = 0x9B,
	WIDGET_SHOP = 0x9C,
	WIDGET_RESPONSE = 0x9D,
	WIDGET_DRIVE_HYBRID = 0x9E,
	WIDGET_REPLAY = 0x9F,
	WIDGET_PLAYER_INFO = 0xA0,
	WIDGET_RADAR = 0xA1,
	WIDGET_HELP_TEXT = 0xA2,
	WIDGET_TAP_METER = 0xA3,
	WIDGET_THUMB_CIRCLE = 0xA4,
	WIDGET_MENU = 0xA5,
	WIDGET_SWIPE_RADIO = 0xA6,
	WIDGET_PED_MOVE = 0xA7,
	WIDGET_SPRINT = 0xA8,
	WIDGET_BASKETBALL_JUMP = 0xA9,
	WIDGET_SHOOT_LOOK = 0xAA,
	WIDGET_VEHICLE_LANECORRECTION = 0xAB,
	WIDGET_VEHICLE_STEER_ANALOG = 0xAC,
	WIDGET_VEHICLE_FLICK = 0xAD,
	WIDGET_CHEATS_RECT = 0xAE,
	WIDGET_LOOK = 0xAF,
	WIDGET_MISSION_TIMERS = 0xB0,
	WIDGET_RACING = 0xB1,
	WIDGET_RACE_RESULTS = 0xB2,
	WIDGET_HORSE_RESULTS = 0xB3,
	WIDGET_WAGER = 0xB4,
	WIDGET_POOL = 0xB5,
	WIDGET_POKER_PAYOUT = 0xB6,
	WIDGET_VITAL_STATS = 0xB7,
	WIDGET_ROULETTE_SPIN = 0xB8,
	WIDGET_AIRPORT_LEFT = 0xB9,
	WIDGET_AIRPORT_RIGHT = 0xBA,
	WIDGET_PARACHUTE = 0xBB,
	WIDGET_TATTOO_PREVIEW = 0xBC,
	WIDGET_SAVE_NOTIFICATION = 0xBD,
	NUM_WIDGETS = 0xBE
};

typedef struct CSprite2d {
	uintptr_t m_pTexture;
};

typedef struct CRect {
	float left = 1000000.0F; // x1
	float top = -1000000.0F; // y1
	float right = -1000000.0F; // x2
	float bottom = 1000000.0F; // y2
};

typedef struct CRGBA {
	uint8_t r{}, g{}, b{}, a{};
};

typedef struct CWidget {
	void* vtable;
	uintptr_t m_HIDMapping;
	CSprite2d m_Sprite;
	float m_fOriginX;
	float m_fOriginY;
	float m_fScaleX;
	float m_fScaleY;
	float m_fFadeRate;
	CRect m_RectScreen;
	float m_fExtraPaddingScale;
	CRect m_RectPaddedScreen;
	float m_fTapHoldTime;
	bool m_bTaphold;
	CRGBA m_Color;
	bool m_bEnabled;
	bool m_bCachedEnabled;
	float m_fTapHistory[10];
	int m_nTouchIndex;
	int m_nFrameCount;
	int m_nFlags;
	float m_fUserData;
	void* m_pHoldEffect;
	float m_fHoldEffectPeriod;
};
static_assert(sizeof(CWidget) == 0x90);

#pragma pack(push, 1)
typedef struct _RES_ENTRY_OBJ {
	PADDING(_pad0, 48); // 0-48
	uintptr_t validate; //48-52
	PADDING(_pad1, 4); //52-56

} RES_ENTRY_OBJ;
static_assert(sizeof(_RES_ENTRY_OBJ) == 56);

typedef struct _RECT {
	float fLeft;
	float fBottom;
	float fRight;
	float fTop;
} RECT;

typedef struct _VECTOR {
	float X, Y, Z;

	_VECTOR()
	{
		X = Y = Z = 0.0f;
	}

	_VECTOR(float f)
	{
		X = Y = Z = f;
	}

	_VECTOR(float x, float y, float z)
	{
		X = x;
		Y = y;
		Z = z;
	}
} VECTOR, * PVECTOR;

typedef struct _MATRIX4X4 {
	VECTOR right;        // 0-12 	; r11 r12 r13
	uint32_t flags;    // 12-16
	VECTOR up;            // 16-28	; r21 r22 r23
	float pad_u;        // 28-32
	VECTOR at;            // 32-44	; r31 r32 r33
	float pad_a;        // 44-48
	VECTOR pos;            // 48-60
	float pad_p;        // 60-64
} MATRIX4X4, * PMATRIX4X4;

typedef struct _WEAPON_SLOT_TYPE {
	uint32_t dwType;
	uint32_t dwState;
	uint32_t dwAmmoInClip;
	uint32_t dwAmmo;
	PADDING(_pwep1, 12);
} WEAPON_SLOT_TYPE;  // MUST BE EXACTLY ALIGNED TO 28 bytes

typedef struct _ENTITY_TYPE {
	// ENTITY STUFF
	uint32_t vtable;            // 0-4		;vtable				- 2.0
	VECTOR vPos;                // 4-16
	float fRotZBeforeMat;        // 16-20
	MATRIX4X4* mat;            // 20-24	;mat				- 2.0
	uintptr_t pdwRenderWare;    // 24-28	;pRWObject			- 2.0
	uint32_t dwProcessingFlags;    // 28-32	;dwProcessingFlags	- 2.0
	PADDING(_pad92, 6);            // 32-38
	uint16_t nModelIndex;        // 38-40	;ModelIndex			- 2.0
	PADDING(_pad93, 18);        // 40-58
	uint8_t nControlFlags;        // 58-59	;nControlFlags		- 2.0
	PADDING(_pad95, 9);            // 59-68
	uint32_t flags;                // 68-72
	VECTOR vecMoveSpeed;        // 72-84	;vecMoveSpeed		- 2.0
	VECTOR vecTurnSpeed;        // 84-96	;vecTurnSpeed		- 2.0
	PADDING(_pad96, 88);        // 96-184
	uintptr_t dwUnkModelRel;    // 184-188	;dwUnkModelRel		- 2.0
} ENTITY_TYPE;

typedef struct _AnimBlendFrameData {
	uint8_t bFlags;
	PADDING(_pad75, 3);
	VECTOR vOffset;
	uintptr_t pInterpFrame;
	uint32_t m_nNodeId;
} AnimBlendFrameData;

typedef struct _PED_TASKS_TYPE {
	uint32_t* pdwPed; //0-4
	// Basic Tasks
	uint32_t* pdwDamage; //4-8
	uint32_t* pdwFallEnterExit; //8-12
	uint32_t* pdwSwimWasted; //12-16
	uint32_t* pdwJumpJetPack; //16-20
	uint32_t* pdwAction; //20-24
	// Extended Tasks
	uint32_t* pdwFighting; //24-28
	uint32_t* pdwCrouching; //28-32
	uint32_t* pdwSay; //32-36
	uint32_t* pdwFacialComplex; //36-40
	uint32_t* pdwPartialAnim; //40-44
	uint32_t* pdwIK; //44-48
} PED_TASKS_TYPE;

enum ePrimaryTasks //array indexes
{
	TASK_PRIMARY_PHYSICAL_RESPONSE = 0,
	TASK_PRIMARY_EVENT_RESPONSE_TEMP,
	TASK_PRIMARY_EVENT_RESPONSE_NONTEMP,
	TASK_PRIMARY_PRIMARY,
	TASK_PRIMARY_DEFAULT,
	TASK_PRIMARY_MAX
};

enum eSecondaryTasks //array indexes
{
	TASK_SECONDARY_ATTACK = 0,                // want duck to be after attack
	TASK_SECONDARY_DUCK,                    // because attack controls ducking movement
	TASK_SECONDARY_SAY,
	TASK_SECONDARY_FACIAL_COMPLEX,
	TASK_SECONDARY_PARTIAL_ANIM,
	TASK_SECONDARY_IK,
	TASK_SECONDARY_MAX
};

class CTaskManager {
public:
	void* m_aPrimaryTasks[5];
	void* m_aSecondaryTasks[6];
	class CPed* m_pPed;
};

class CPedIntelligence {
public:
	class CPed* m_pPed;
	CTaskManager m_TaskMgr;
};

typedef struct _PED_TYPE {
	ENTITY_TYPE entity;                // 0000-0188	;entity				- 2.0
	PADDING(_pad100, 900);                // 0188-1088
	PED_TASKS_TYPE* Tasks;                // 1088-1092
	uintptr_t dwPlayerInfoOffset;        // 1092-1096	;dwPlayerInfoOffset - 2.0
	PADDING(_pad106, 4);                // 1096-1100
	uint32_t dwAction;                    // 1100-1104	;Action				- 2.0
	PADDING(_pad101, 52);                // 1104-1156
	uint32_t dwStateFlags;                // 1156-1160	;StateFlags		- надо тестить
	PADDING(_pad102, 12);                // 1160-1172
	AnimBlendFrameData* m_pPedBones[19];// 1172-1248‬
	PADDING(_pad174, 100);                // 1248-1348
	float fHealth;                        // 1348-1352	;Health				- 2.0
	float fMaxHealth;                    // 1352-1356	;MaxHealth			- 2.0
	float fArmour;                        // 1356-1360	;Armour				- 2.0
	PADDING(_pad103, 12);                // 1360-1372
	float fRotation1;                    // 1372-1376	;Rotation1			- 2.0
	float fRotation2;                    // 1376-1380	;Rotation2			- 2.0
	PADDING(_pad104, 44);                // 1380-1424
	uintptr_t pVehicle;                    // 1424-1428	;pVehicle			- 2.0
	PADDING(_pad105, 8);                // 1428-1436
	uint32_t dwPedType;                    // 1436-1440	;dwPedType			- 2.0
	PADDING(_pad107, 4);                // 1440-1444
	WEAPON_SLOT_TYPE WeaponSlots[13];    // 1444-1808	;WeaponSlots		- 2.0
	PADDING(_pad108, 12);                // 1808-1820
	uint8_t byteCurWeaponSlot;            // 1820-1821	;byteCurWeaponSlot	- 2.0
	PADDING(_pad109, 75);                // 1821-1896
	uint32_t dwWeaponUsed;                // 1896-1900	;dwWeaponUsed		- 2.0
	ENTITY_TYPE* pdwDamageEntity;        // 1900-1904	;pdwDamageEntity	- 2.0
} PED_TYPE;

typedef struct _VEHICLE_TYPE {
	ENTITY_TYPE entity;                // 0000-0188	;entity
	PADDING(_pad200, 880);            // 0188-1068
	uint8_t byteFlags;                // 1068-1069	;byteFlags			-
	PADDING(_pad210, 4);            // 1069-1073
	uint8_t bSirenOn;                // 1073-1074
	PADDING(_pad209, 6);            // 1074-1080
	uint8_t byteColor1;                // 1080-1081	;byteColor1			- 2.0
	uint8_t byteColor2;                // 1081-1082	;byteColor2			- 2.0
	uint8_t byteColor3;                // 1082-1083	;byteColor3			- 2.0
	uint8_t byteColor4;                // 1083-1084	;byteColor4			- 2.0
	PADDING(_pad206, 36);            // 1084-1124
	uint16_t wAlarmState;            // 1120-1122	;wAlarmState		- 2.0
	PADDING(_pad207, 2);            // 1122-1124
	PED_TYPE* pDriver;                // 1124-1128	;driver				- 2.0
	PED_TYPE* pPassengers[7];        // 1128-1156	;pPassengers		- 2.0
	PADDING(_pad201, 8);            // 1156-1164
	uint8_t byteMaxPassengers;        // 1164-1165	;byteMaxPassengers	- 2.0
	PADDING(_pad205, 47);            // 1165-1212
	float fDirtLevel;                // 1212-1216	;fDirtLevel			- 2.0
	PADDING(_pad203, 12);            // 1216-1228
	float fHealth;                    // 1228-1232	;fHealth			- 2.0
	_VEHICLE_TYPE* pTractor;        // 1232-1236	;pTractor			- 2.0
	_VEHICLE_TYPE* pTrailer;        // 1236-1240	;pTrailer			- 2.0
	PADDING(_pad208, 48);            // 1240-1288
	uint32_t dwDoorsLocked;            // 1288-1292	;dwDoorsLocked		- 2.0
	PADDING(_pad202, 172);            // 1292-1464
	union {
		struct {
			PADDING(_pad245, 1);        // 1464-1465
			uint8_t byteTireStatus[4]; // 1465-1469
			uint8_t byteDoorStatus[6];    // 1469-1475
			uint8_t byteDamageUnk;        // 1475-1476
			uint32_t dwLightStatus;        // 1476-1480
			uint32_t dwPanelStatus;        // 1480-1484
		};
		struct {
			float fTrainSpeed;            // 1464-1468
			PADDING(_pad204, 44);        // 1468-1512
		};
	};
	_VEHICLE_TYPE* pNextCarriage;    // 1512-1516	;pNextCarriage		- 2.0
	PADDING(_pad100, 132);            // 1516-1648
	uint8_t byteBikeTireStatus[2];    // 1648-1650
	PADDING(_pad101, 526);            // 1650-2176
	uint16_t wHydraThrusters;        // 2176-2178	;wHydraThrusters
} VEHICLE_TYPE;
#pragma pack(pop)

#define    VEHICLE_SUBTYPE_CAR                1
#define    VEHICLE_SUBTYPE_BIKE            2
#define    VEHICLE_SUBTYPE_HELI            3
#define    VEHICLE_SUBTYPE_BOAT            4
#define    VEHICLE_SUBTYPE_PLANE            5
#define    VEHICLE_SUBTYPE_PUSHBIKE        6
#define    VEHICLE_SUBTYPE_TRAIN            7

#define TRAIN_PASSENGER_LOCO            538
#define TRAIN_FREIGHT_LOCO                537
#define TRAIN_PASSENGER                    570
#define TRAIN_FREIGHT                    569
#define TRAIN_TRAM                        449
#define HYDRA                            520

#define ACTION_WASTED                    55
#define ACTION_DEATH                    54
#define ACTION_INCAR                    50
#define ACTION_NORMAL                    1
#define ACTION_SCOPE                    12
#define ACTION_NONE                        0

#define WEAPON_FIST                        0
#define WEAPON_BRASSKNUCKLE                1
#define WEAPON_GOLFCLUB                    2
#define WEAPON_NITESTICK                3
#define WEAPON_KNIFE                    4
#define WEAPON_BAT                        5
#define WEAPON_SHOVEL                    6
#define WEAPON_POOLSTICK                7
#define WEAPON_KATANA                    8
#define WEAPON_CHAINSAW                    9
#define WEAPON_DILDO                    10
#define WEAPON_DILDO2                    11
#define WEAPON_VIBRATOR                    12
#define WEAPON_VIBRATOR2                13
#define WEAPON_FLOWER                    14
#define WEAPON_CANE                        15
#define WEAPON_GRENADE                    16
#define WEAPON_TEARGAS                    17
#define WEAPON_MOLTOV                    18
#define WEAPON_ROCKET                    19
#define WEAPON_ROCKET_HS                20
#define WEAPON_FREEFALLBOMB                21
#define WEAPON_COLT45                    22
#define WEAPON_SILENCED                    23
#define WEAPON_DEAGLE                    24
#define WEAPON_SHOTGUN                    25
#define WEAPON_SAWEDOFF                    26
#define WEAPON_SHOTGSPA                    27
#define WEAPON_UZI                        28
#define WEAPON_MP5                        29
#define WEAPON_AK47                        30
#define WEAPON_M4                        31
#define WEAPON_TEC9                        32
#define WEAPON_RIFLE                    33
#define WEAPON_SNIPER                    34
#define WEAPON_ROCKETLAUNCHER            35
#define WEAPON_HEATSEEKER                36
#define WEAPON_FLAMETHROWER                37
#define WEAPON_MINIGUN                    38
#define WEAPON_SATCHEL                    39
#define WEAPON_BOMB                        40
#define WEAPON_SPRAYCAN                    41
#define WEAPON_FIREEXTINGUISHER            42
#define WEAPON_CAMERA                    43
#define WEAPON_NIGHTVISION                44
#define WEAPON_INFRARED                    45
#define WEAPON_PARACHUTE                46
#define WEAPON_ARMOUR                    47
#define WEAPON_VEHICLE                    49
#define WEAPON_HELIBLADES                50
#define WEAPON_EXPLOSION                51
#define WEAPON_DROWN                    53
#define WEAPON_COLLISION                54

#define WEAPON_MODEL_BRASSKNUCKLE        331 // was 332
#define WEAPON_MODEL_GOLFCLUB            333
#define WEAPON_MODEL_NITESTICK            334
#define WEAPON_MODEL_KNIFE                335
#define WEAPON_MODEL_BAT                336
#define WEAPON_MODEL_SHOVEL                337
#define WEAPON_MODEL_POOLSTICK            338
#define WEAPON_MODEL_KATANA                339
#define WEAPON_MODEL_CHAINSAW            341
#define WEAPON_MODEL_DILDO                321
#define WEAPON_MODEL_DILDO2                322
#define WEAPON_MODEL_VIBRATOR            323
#define WEAPON_MODEL_VIBRATOR2            324
#define WEAPON_MODEL_FLOWER                325
#define WEAPON_MODEL_CANE                326
#define WEAPON_MODEL_GRENADE            342 // was 327
#define WEAPON_MODEL_TEARGAS            343 // was 328
#define WEAPON_MODEL_MOLOTOV            344 // was 329
#define WEAPON_MODEL_COLT45                346
#define WEAPON_MODEL_SILENCED            347
#define WEAPON_MODEL_DEAGLE                348
#define WEAPON_MODEL_SHOTGUN            349
#define WEAPON_MODEL_SAWEDOFF            350
#define WEAPON_MODEL_SHOTGSPA            351
#define WEAPON_MODEL_UZI                352
#define WEAPON_MODEL_MP5                353
#define WEAPON_MODEL_AK47                355
#define WEAPON_MODEL_M4                    356
#define WEAPON_MODEL_TEC9                372
#define WEAPON_MODEL_RIFLE                357
#define WEAPON_MODEL_SNIPER                358
#define WEAPON_MODEL_ROCKETLAUNCHER        359
#define WEAPON_MODEL_HEATSEEKER            360
#define WEAPON_MODEL_FLAMETHROWER        361
#define WEAPON_MODEL_MINIGUN            362
#define WEAPON_MODEL_SATCHEL            363
#define WEAPON_MODEL_BOMB                364
#define WEAPON_MODEL_SPRAYCAN            365
#define WEAPON_MODEL_FIREEXTINGUISHER    366
#define WEAPON_MODEL_CAMERA                367
#define WEAPON_MODEL_NIGHTVISION        368    // newly added
#define WEAPON_MODEL_INFRARED            369    // newly added
#define WEAPON_MODEL_JETPACK            370    // newly added
#define WEAPON_MODEL_PARACHUTE            371

#define OBJECT_PARACHUTE                3131
#define OBJECT_CJ_CIGGY                    1485
#define OBJECT_DYN_BEER_1                1486
#define OBJECT_CJ_BEER_B_2                1543
#define OBJECT_CJ_PINT_GLASS            1546
