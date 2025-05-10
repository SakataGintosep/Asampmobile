#include "../main.h"
#include "../samp.h"
#include "common.h"

bool g_disableVehicleCollisions = false;

DECL_HOOK(void, NvUtilInit)
{
	LOGI("NvUtilInit_hook()");
	NvUtilInit();

	// 006D687C             ; char StorageRootBuffer[512]
}

DECL_HOOK(void, Render2dStuff)
{
	Render2dStuff();
}

DECL_HOOK(uint32_t, CAutoMobile_ProcessEntityCollision, VEHICLE_TYPE* __thisVehicle, VEHICLE_TYPE* _pCollidedVehicle, uintptr_t a3)
{
	if (!__thisVehicle) {
		LOGE("Invalid _this pointer in CAutoMobile_ProcessEntityCollision_hook. Skipping hook.");
		return 0;
	}

	if (!SAMP::netgame() ||
			!g_disableVehicleCollisions ||
			!__thisVehicle ||
			!_pCollidedVehicle ||
			_pCollidedVehicle->entity.nModelIndex < 400 ||
			_pCollidedVehicle->entity.nModelIndex > 611 ||
			!__thisVehicle->pDriver ||
			!_pCollidedVehicle->pDriver) {
		return CAutoMobile_ProcessEntityCollision(__thisVehicle, _pCollidedVehicle, a3);
	}

	return 0;
}

DECL_HOOK(uint32_t, CBike_ProcessEntityCollision, VEHICLE_TYPE* __thisVehicle, VEHICLE_TYPE* _pCollidedVehicle, uintptr_t a3)
{
	if (!__thisVehicle) {
		LOGE("Invalid _this pointer in CBike_ProcessEntityCollision_hook. Skipping hook.");
		return 0;
	}

	if (!SAMP::netgame() ||
			!g_disableVehicleCollisions ||
			!__thisVehicle ||
			!_pCollidedVehicle ||
			_pCollidedVehicle->entity.nModelIndex < 400 ||
			_pCollidedVehicle->entity.nModelIndex > 611 ||
			!__thisVehicle->pDriver ||
			!_pCollidedVehicle->pDriver) {
		return CBike_ProcessEntityCollision(__thisVehicle, _pCollidedVehicle, a3);
	}

	return 0;
}

DECL_HOOK(uint32_t, CMonsterTruck_ProcessEntityCollision, VEHICLE_TYPE* __thisVehicle, VEHICLE_TYPE* _pCollidedVehicle, uintptr_t a3)
{
	if (!__thisVehicle) {
		LOGE("Invalid _this pointer in CMonsterTruck_ProcessEntityCollision_hook. Skipping hook.");
		return 0;
	}

	if (!SAMP::netgame() ||
			!g_disableVehicleCollisions ||
			!__thisVehicle ||
			!_pCollidedVehicle ||
			_pCollidedVehicle->entity.nModelIndex < 400 ||
			_pCollidedVehicle->entity.nModelIndex > 611 ||
			!__thisVehicle->pDriver ||
			!_pCollidedVehicle->pDriver) {
		return CMonsterTruck_ProcessEntityCollision(__thisVehicle, _pCollidedVehicle, a3);
	}

	return 0;
}

DECL_HOOK(uint32_t, CTrailer_ProcessEntityCollision, VEHICLE_TYPE* __thisVehicle, VEHICLE_TYPE* _pCollidedVehicle, uintptr_t a3)
{
	if (!__thisVehicle) {
		LOGE("Invalid _this pointer in CTrailer_ProcessEntityCollision_hook. Skipping hook.");
		return 0;
	}

	if (!SAMP::netgame() ||
			!g_disableVehicleCollisions ||
			!__thisVehicle ||
			!_pCollidedVehicle ||
			_pCollidedVehicle->entity.nModelIndex < 400 ||
			_pCollidedVehicle->entity.nModelIndex > 611 ||
			!__thisVehicle->pDriver ||
			!_pCollidedVehicle->pDriver) {
		return CTrailer_ProcessEntityCollision(__thisVehicle, _pCollidedVehicle, a3);
	}

	return 0;
}

DECL_HOOK(void, CAEVehicleAudioEntity_ProcessVehicle, uintptr_t _this, VEHICLE_TYPE* pVehicle)
{
	if (!_this) {
		LOGE("Invalid _this pointer in CAEVehicleAudioEntity_ProcessVehicle_hook. Skipping hook.");
		return;
	}

	CAEVehicleAudioEntity_ProcessVehicle(_this, pVehicle);

	// fix engine sound
	if (SAMP::game()) {
		if (SAMP::game()->findPlayerPed()->isInVehicle() && SAMP::game()->findPlayerPed()->getGtaVehicle() == pVehicle) {
			// todo
		}
	}
}


void initializeSAHooks()
{
	LOGI("initializeSAHooks()");

	// NvUtilInit
	//HOOK(SA_Addr(0x26919C), NvUtilInit);

	// Render2dStuff
	//HOOK(SA_Addr(0x3F641C), Render2dStuff);

	// CAEVehicleAudioEntity::ProcessVehicle(CPhysical *)
	//HOOK(SA_Addr(0x3AE3D8), CAEVehicleAudioEntity_ProcessVehicle);

	// disable vehicle collisions rpc
	HOOK(SA_Addr(0x55BEA0), CAutoMobile_ProcessEntityCollision);
	HOOK(SA_Addr(0x5661D4), CBike_ProcessEntityCollision);
	HOOK(SA_Addr(0x575600), CMonsterTruck_ProcessEntityCollision);
	HOOK(SA_Addr(0x57C084), CTrailer_ProcessEntityCollision);
}
