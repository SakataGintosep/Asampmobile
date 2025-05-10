#include "utils.h"
#include "../main.h"
#include "../samp.h"

#define PI 3.14159265

namespace utils {
std::string int_to_hex(int val)
{
	char hex[20];
	sprintf(hex, "%02X", val);
	return std::string(hex);
}

PED_TYPE* GamePool_FindPlayerPed()
{
	// WORLD_PLAYERS
	return *(PED_TYPE**) *(char**) (SA_Addr(0x6783C8));
}

PED_TYPE* GamePool_Ped_GetAt(int iID)
{
	// GetPoolPed
	return ((PED_TYPE* (*)(int)) (SA_Addr(0x483D48 + 1)))(iID);
}

int GamePool_Ped_GetIndex(PED_TYPE* pActor)
{
	// GettPoolPedRef
	return ((int (*)(PED_TYPE*)) (SA_Addr(0x483D3A + 1)))(pActor);
}

ENTITY_TYPE* GamePool_Object_GetAt(int iID)
{
	// GetPoolObj
	return ((ENTITY_TYPE* (*)(int)) (SA_Addr(0x483D62 + 1)))(iID);
}

uintptr_t GamePool_Vehicle_GetIndex(VEHICLE_TYPE* pGtaVehicle)
{
	// GettPoolVehicleRef
	return ((uintptr_t(*)(VEHICLE_TYPE * ))(SA_Addr(0x483D20 + 1)))(pGtaVehicle);
}

VEHICLE_TYPE* GamePool_Vehicle_GetAt(int iID)
{
	// GetPoolVehicle
	return ((VEHICLE_TYPE* (*)(int)) (SA_Addr(0x483D2E + 1)))(iID);
}

int GetVehicleSubtype(VEHICLE_TYPE* pGtaVehicle)
{
	if (pGtaVehicle) {
		uintptr_t vtable = pGtaVehicle->entity.vtable;

		if (vtable == SA_Addr(0x66D688)) { // CAutomobile
			return VEHICLE_SUBTYPE_CAR;
		}
		else if (vtable == SA_Addr(0x66D800)) { // CBike
			return VEHICLE_SUBTYPE_BIKE;
		}
		else if (vtable == SA_Addr(0x66DB44)) { // CHeli
			return VEHICLE_SUBTYPE_HELI;
		}
		else if (vtable == SA_Addr(0x66DA30)) { // CBoat
			return VEHICLE_SUBTYPE_BOAT;
		}
		else if (vtable == SA_Addr(0x66DD94)) { // CPlane
			return VEHICLE_SUBTYPE_PLANE;
		}
		else if (vtable == SA_Addr(0x66D918)) { // CBmx
			return VEHICLE_SUBTYPE_PUSHBIKE;
		}
		else if (vtable == SA_Addr(0x66E10C)) { // CTrain
			return VEHICLE_SUBTYPE_TRAIN;
		}
	}

	return 0;
}

uintptr_t GetModelInfoByID(int iModelID)
{
	if (iModelID < 0 || iModelID > 20000) {
		return false;
	}

	uintptr_t* dwModelArray = (uintptr_t * )(SA_Addr(0x91DCB8));
	return dwModelArray[iModelID];
}

bool IsExistInfoForModel(int iModelID)
{
	return GetModelInfoByID(iModelID) != 0;
}

bool IsValidModel(int iModelID)
{
	uintptr_t pModel = GetModelInfoByID(iModelID);

	if (pModel && *(uintptr_t*) pModel != 0xFFFFFFEC) {
		return true;
	}

	return false;
}

int GetModelRefCounts(int iModel)
{
	uint16_t* p = (uint16_t * )(GetModelInfoByID(iModel) + 30);
	return *p;
}

bool IsValidPedModel(uint modelID)
{
	if (modelID >= 0 && modelID <= 30000) {
		uintptr_t modelInfo = GetModelInfoByID(modelID);
		if (modelInfo) {
			if ((*(uintptr_t*) modelInfo) == (uintptr_t)(SA_Addr(0x667668))) {
				return true;
			}
		}
	}

	return false;
}

uintptr_t GetModelRWObject(uint uiModel)
{
	uintptr_t modelInfo = GetModelInfoByID(uiModel);
	return *(uintptr_t * )(modelInfo + 0x34); /* pRWObject */
}

RwTexture* GetTexture(const char* texname)
{
	RwTexture* texture = ((RwTexture* (*)(const char*)) (SA_Addr(0x1E9CE4 + 1)))(texname);
	if (texture == 0) {
		return 0;
	}
	int count = *(int*) (texture + 0x54);
	count++;
	*(int*) (texture + 0x54) = count;
	return texture;
}

RwTexture* LoadTextureFromDB(const char* dbname, const char* texname)
{
	RwTexture* texture = 0x00;

	// TextureDatabaseRuntime::GetDatabase(dbname)
	uintptr_t
	db_handle = ((uintptr_t(*)(
	const char*)) (SA_Addr(0x1EAC8C + 1)))(dbname);

	if (db_handle == 0x00) {
		LOGI("TextureDatabase %s not found!", dbname);
		goto ret;
	}

	// TextureDatabaseRuntime::Register(db)
	((void (*)(uintptr_t)) (SA_Addr(0x1E9BC8 + 1)))(db_handle);

	texture = GetTexture(texname);
	if (texture == 0x00) {
		LOGI("Texture (%s) not found in database (%s)", texname, dbname);
	}

	// TextureDatabaseRuntime::Unregister(db)
	((void (*)(uintptr_t)) (SA_Addr(0x1E9C80 + 1)))(db_handle);

ret:
	return texture;
}

float FloatOffset(float f1, float f2)
{
	if (f1 >= f2) {
		return f1 - f2;
	}
	else {
		return (f2 - f1);
	}
}

float DegToRad(float fDegrees)
{
	if (fDegrees > 360.0f || fDegrees < 0.0f) {
		return 0.0f;
	}

	if (fDegrees > 180.0f) {
		return (float) (-(PI - (((fDegrees - 180.0f) * PI) / 180.0f)));
	}
	else {
		return (float) ((fDegrees * PI) / 180.0f);
	}
}

float fixAngle(float angle)
{
	if (angle > 180.0f) {
		angle -= 360.0f;
	}
	if (angle < -180.0f) {
		angle += 360.0f;
	}

	return angle;
}

float subAngle(float a1, float a2)
{
	return fixAngle(fixAngle(a2) - a1);
}

void CSprite2d_Constructor(RwTexture* pTexture)
{
	// CSprite2d::CSprite2d(void)
	Memory::callFunction<void>(SA_Addr(0x5C8800 + 1), &pTexture);
}

void CSprite2d_Destructor(RwTexture* pTexture)
{
	// CSprite2d::~CSprite2d()
	Memory::callFunction<void>(SA_Addr(0x5C8806 + 1), &pTexture);
}

RwTexture* CSprite2d_SetTexture(const char* szTexture)
{
	RwTexture* pTexture = nullptr;

	// CSprite2d::SetTexture(char *)
	Memory::callFunction<void>(SA_Addr(0x5C8838 + 1), &pTexture, szTexture);

	return pTexture;
}

void CSprite2d_Draw(RwTexture* pTexture, RECT* rect, uint32_t dwColor, float* uv)
{
	// CSprite2d::Draw(CRect const&,CRGBA const&,float,float,float,float,float,float,float,float)
	Memory::callFunction<void>(SA_Addr(0x5C95C0 + 1), pTexture, rect, &dwColor, uv[0], uv[1], uv[2],
			uv[3], uv[4], uv[5], uv[6], uv[7]);
}

void DrawRaster(RwRaster* raster, RECT const& rect)
{
	uint32_t white = 0xFFFFFFFF;
	// CSprite2d::SetVertices(rect, color::White, color::White, color::White, color::White);
	((void (*)(RwRaster*, const RECT&, uint32_t, uint32_t, uint32_t, uint32_t)) (SA_Addr(
			0x5C9014 + 1)))(raster, rect, white, white, white, white);
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, raster);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*) true);
	RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, (RwIm2DVertex*) (SA_Addr(0xA7C264)), 4);
}

void DrawTextureUV(RwTexture* texture, RECT* rect, uint32_t dwColor, float* uv)
{
	if (texture) {
		RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*) rwFILTERLINEAR);

		CSprite2d_Draw(texture, rect, dwColor, uv);
	}
}

float GetDistance(VECTOR vec1, VECTOR vec2)
{
	return sqrt(
			(vec1.Z - vec2.Z) * (vec1.Z - vec2.Z) +
					(vec1.Y - vec2.Y) * (vec1.Y - vec2.Y) +
					(vec1.X - vec2.X) * (vec1.X - vec2.X));
}

int GameGetWeaponModelIDFromWeaponID(int iWeaponID)
{
	switch (iWeaponID) {
		case WEAPON_BRASSKNUCKLE: return WEAPON_MODEL_BRASSKNUCKLE;

		case WEAPON_GOLFCLUB: return WEAPON_MODEL_GOLFCLUB;

		case WEAPON_NITESTICK: return WEAPON_MODEL_NITESTICK;

		case WEAPON_KNIFE: return WEAPON_MODEL_KNIFE;

		case WEAPON_BAT: return WEAPON_MODEL_BAT;

		case WEAPON_SHOVEL: return WEAPON_MODEL_SHOVEL;

		case WEAPON_POOLSTICK: return WEAPON_MODEL_POOLSTICK;

		case WEAPON_KATANA: return WEAPON_MODEL_KATANA;

		case WEAPON_CHAINSAW: return WEAPON_MODEL_CHAINSAW;

		case WEAPON_DILDO: return WEAPON_MODEL_DILDO;

		case WEAPON_DILDO2: return WEAPON_MODEL_DILDO2;

		case WEAPON_VIBRATOR: return WEAPON_MODEL_VIBRATOR;

		case WEAPON_VIBRATOR2: return WEAPON_MODEL_VIBRATOR2;

		case WEAPON_FLOWER: return WEAPON_MODEL_FLOWER;

		case WEAPON_CANE: return WEAPON_MODEL_CANE;

		case WEAPON_GRENADE: return WEAPON_MODEL_GRENADE;

		case WEAPON_TEARGAS: return WEAPON_MODEL_TEARGAS;

		case WEAPON_MOLTOV: return WEAPON_MODEL_MOLOTOV;

		case WEAPON_COLT45: return WEAPON_MODEL_COLT45;

		case WEAPON_SILENCED: return WEAPON_MODEL_SILENCED;

		case WEAPON_DEAGLE: return WEAPON_MODEL_DEAGLE;

		case WEAPON_SHOTGUN: return WEAPON_MODEL_SHOTGUN;

		case WEAPON_SAWEDOFF: return WEAPON_MODEL_SAWEDOFF;

		case WEAPON_SHOTGSPA: return WEAPON_MODEL_SHOTGSPA;

		case WEAPON_UZI: return WEAPON_MODEL_UZI;

		case WEAPON_MP5: return WEAPON_MODEL_MP5;

		case WEAPON_AK47: return WEAPON_MODEL_AK47;

		case WEAPON_M4: return WEAPON_MODEL_M4;

		case WEAPON_TEC9: return WEAPON_MODEL_TEC9;

		case WEAPON_RIFLE: return WEAPON_MODEL_RIFLE;

		case WEAPON_SNIPER: return WEAPON_MODEL_SNIPER;

		case WEAPON_ROCKETLAUNCHER: return WEAPON_MODEL_ROCKETLAUNCHER;

		case WEAPON_HEATSEEKER: return WEAPON_MODEL_HEATSEEKER;

		case WEAPON_FLAMETHROWER: return WEAPON_MODEL_FLAMETHROWER;

		case WEAPON_MINIGUN: return WEAPON_MODEL_MINIGUN;

		case WEAPON_SATCHEL: return WEAPON_MODEL_SATCHEL;

		case WEAPON_BOMB: return WEAPON_MODEL_BOMB;

		case WEAPON_SPRAYCAN: return WEAPON_MODEL_SPRAYCAN;

		case WEAPON_FIREEXTINGUISHER: return WEAPON_MODEL_FIREEXTINGUISHER;

		case WEAPON_CAMERA: return WEAPON_MODEL_CAMERA;

		case WEAPON_NIGHTVISION: return WEAPON_MODEL_NIGHTVISION;

		case WEAPON_INFRARED: return WEAPON_MODEL_INFRARED;

		case WEAPON_PARACHUTE: return WEAPON_MODEL_PARACHUTE;
	}

	return -1;
}

bool IsPointInRect(float x, float y, RECT* rect)
{
	if (x >= rect->fLeft && x <= rect->fRight &&
			y >= rect->fBottom && y <= rect->fTop) {
		return true;
	}

	return false;
}

uintptr_t ModelInfoCreateInstance(int iModel)
{
	uintptr_t modelInfo = GetModelInfoByID(iModel);
	if (modelInfo) {
		return ((uintptr_t(*)(uintptr_t)) * (uintptr_t * )(*(uintptr_t*) modelInfo + 0x2C))(
				modelInfo);
	}

	return 0;
}

void RenderClumpOrAtomic(uintptr_t rwObject)
{
	if (rwObject) {
		if (*(uint8_t*) rwObject == 1) {
			// Atomic
			LOGI("Render Atomic!");
			((void (*)(uintptr_t)) (*(uintptr_t * )(rwObject + 0x48)))(rwObject);
		}
		else if (*(uint8_t*) rwObject == 2) {
			LOGI("Render Clump!");
			// rpClumpRender
			((void (*)(uintptr_t)) (SA_Addr(0x2142DC + 1)))(rwObject);
		}
	}
}

float GetModelColSphereRadius(int iModel)
{
	uintptr_t modelInfo = GetModelInfoByID(iModel);

	if (modelInfo) {
		uintptr_t colModel = *(uintptr_t * )(modelInfo + 0x2C);
		if (colModel != 0) {
			return *(float*) (colModel + 0x24);
		}
	}

	return 0.0f;
}

void GetModelColSphereVecCenter(int iModel, VECTOR* vec)
{
	uintptr_t modelInfo = GetModelInfoByID(iModel);

	if (modelInfo) {
		uintptr_t colModel = *(uintptr_t * )(modelInfo + 0x2C);
		if (colModel != 0) {
			VECTOR* v = (VECTOR*) (colModel + 0x18);

			vec->X = v->X;
			vec->Y = v->Y;
			vec->Z = v->Z;
		}
	}
}

void DestroyAtomicOrClump(uintptr_t rwObject)
{
	if (rwObject) {
		int type = *(int*) (rwObject);

		if (type == 1) {
			// RpAtomicDestroy
			((void (*)(uintptr_t)) (SA_Addr(0x2141EC + 1)))(rwObject);

			uintptr_t parent = *(uintptr_t * )(rwObject + 4);
			if (parent) {
				// RwFrameDestroy
				((void (*)(uintptr_t)) (SA_Addr(0x1D846C + 1)))(parent);
			}
		}
		else if (type == 2) {
			// RpClumpDestroy
			((void (*)(uintptr_t)) (SA_Addr(0x21460C + 1)))(rwObject);
		}
	}
}

void ProjectMatrix(VECTOR* vecOut, MATRIX4X4* mat, VECTOR* vecPos)
{
	vecOut->X =
			mat->at.X * vecPos->Z + mat->up.X * vecPos->Y + mat->right.X * vecPos->X + mat->pos.X;
	vecOut->Y =
			mat->at.Y * vecPos->Z + mat->up.Y * vecPos->Y + mat->right.Y * vecPos->X + mat->pos.Y;
	vecOut->Z =
			mat->at.Z * vecPos->Z + mat->up.Z * vecPos->Y + mat->right.Z * vecPos->X + mat->pos.Z;
}

static VECTOR _axis[3] = {
		{1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f}};

void RwMatrixRotate(MATRIX4X4* mat, int axis, float angle)
{
	((void (*)(MATRIX4X4*, VECTOR*, float, int)) (SA_Addr(0x1E3974 + 1)))(mat, &_axis[axis], angle,
			1);
}

void RwMatrixScale(MATRIX4X4* matrix, VECTOR* scale)
{
	matrix->right.X *= scale->X;
	matrix->right.Y *= scale->X;
	matrix->right.Z *= scale->X;

	matrix->up.X *= scale->Y;
	matrix->up.Y *= scale->Y;
	matrix->up.Z *= scale->Y;

	matrix->at.X *= scale->Z;
	matrix->at.Y *= scale->Z;
	matrix->at.Z *= scale->Z;

	matrix->flags &= 0xFFFDFFFC;
}

void DefinedState()
{
	((void (*)(void)) (SA_Addr(0x5D0BC0 + 1)))();
}

void SetRenderWareCamera(RwCamera* camera)
{
	// CVisibilityPlugins::SetRenderWareCamera
	return ((void (*)(RwCamera*)) (SA_Addr(0x5D61F8 + 1)))(camera);
}

void rpWorldAddCamera(RwCamera* camera)
{
	RpWorld* rpWorld = *(RpWorld**) (SA_Addr(0x9FC938)); // Scene
	if (rpWorld) {
		RpWorldAddCamera(rpWorld, camera);
	}
}

void rpWorldAddLight(RpLight* rpLight)
{
	RpWorld* rpWorld = *(RpWorld**) (SA_Addr(0x9FC938)); // Scene
	if (rpWorld) {
		RpWorldAddLight(rpWorld, rpLight);
	}
}

void rpWorldRemoveLight(RpLight* rpLight)
{
	RpWorld* rpWorld = *(RpWorld**) (SA_Addr(0x9FC938)); // Scene
	if (rpWorld) {
		RpWorldRemoveLight(rpWorld, rpLight);
	}
}

uint32_t GetTickCount()
{
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int CTxdStore_FindTxdSlot(const char* txdname)
{
	// CTxdStore::FindTxdSlot
	return Memory::callFunction<int>(SA_Addr(0x5D3E60 + 1), txdname);
}

void CTxdStore_PushCurrentTxd()
{
	// CTxdStore::PushCurrentTxd
	Memory::callFunction<void>(SA_Addr(0x5D4184 + 1));
}

void CTxdStore_SetCurrentTxd(int slot)
{
	// CTxdStore::SetCurrentTxd
	Memory::callFunction<void>(SA_Addr(0x5D40F4 + 1), slot, 0);
}

void CTxdStore_PopCurrentTxd()
{
	// CTxdStore::PopCurrentTxd
	Memory::callFunction<void>(SA_Addr(0x5D41C4 + 1));
}

int CTxdStore_AddTxdSlot(char const* szTxdName, char const* szTexdb)
{
	// CTxdStore::AddTxdSlot
	Memory::callFunction<int>(SA_Addr(0x5D3B34 + 1), szTxdName, szTexdb, false);
}

void CTxdStore_AddRef(int iTxdSlot)
{
	// CTxdStore::AddRef
	Memory::callFunction<void>(SA_Addr(0x5D3FF0 + 1), iTxdSlot);
}

RwTexture* LoadTextureFromTxd(const char* txdnname, const char* texturename)
{
	RwTexture* pTexture = nullptr;

	if (txdnname && texturename) {
		int iTxdSlot = CTxdStore_FindTxdSlot("*");
		if (iTxdSlot != -1) {
			CTxdStore_PushCurrentTxd();
			CTxdStore_SetCurrentTxd(iTxdSlot);

			pTexture = CSprite2d_SetTexture(texturename);
			if (pTexture) {
				++pTexture->refCount;
			}

			CTxdStore_PopCurrentTxd();
		}
	}

	if (!pTexture)
		LOGI("Texture \"%s\" was not found!", texturename);

	return pTexture;
}
}
