#pragma once

#include <string>

#include "common.h"
#include "rw/rw.h"

namespace utils {
std::string int_to_hex(int val);

PED_TYPE* GamePool_FindPlayerPed();
PED_TYPE* GamePool_Ped_GetAt(int iID);
int GamePool_Ped_GetIndex(PED_TYPE* pActor);
ENTITY_TYPE* GamePool_Object_GetAt(int iID);
uintptr_t GamePool_Vehicle_GetIndex(VEHICLE_TYPE* pGtaVehicle);
VEHICLE_TYPE* GamePool_Vehicle_GetAt(int iID);

int GetVehicleSubtype(VEHICLE_TYPE* pGtaVehicle);

uintptr_t GetModelInfoByID(int iModelID);
bool IsExistInfoForModel(int iModelID);
bool IsValidModel(int iModelID);
int GetModelRefCounts(int iModel);
bool IsValidPedModel(uintptr_t modelID);
uintptr_t GetModelRWObject(uintptr_t modelID);

float FloatOffset(float f1, float f2);
float GetDistance(VECTOR vec1, VECTOR vec2);
float DegToRad(float fDegress);
float fixAngle(float angle);
float subAngle(float a1, float a2);

void SetPlayerPedPtrRecord(uint8_t bytePlayer, PED_TYPE* pPed);
uint8_t FindPlayerNumFromPedPtr(PED_TYPE* dwPedPtr);

int GameGetWeaponModelIDFromWeaponID(int iWeaponID);

void CSprite2d_Constructor(RwTexture* pTexture);
void CSprite2d_Destructor(RwTexture* pTexture);
RwTexture* CSprite2d_SetTexture(const char* szTexture);
void CSprite2d_Draw(RwTexture* pTexture, RECT* rect, uint32_t dwColor, float* uv);

void DrawTextureUV(RwTexture* texture, RECT* rect, uint32_t dwColor, float* uv);

RwTexture* LoadTextureFromDB(const char* dbname, const char* texname);

bool IsPointInRect(float x, float y, RECT* rect);

uintptr_t ModelInfoCreateInstance(int iModel);

void RenderClumpOrAtomic(uintptr_t rwObject);

float GetModelColSphereRadius(int iModel);

void GetModelColSphereVecCenter(int iModel, VECTOR* vec);

void DestroyAtomicOrClump(uintptr_t rwObject);

void ProjectMatrix(VECTOR* vecOut, MATRIX4X4* mat, VECTOR* vecPos);
void RwMatrixRotate(MATRIX4X4* mat, int axis, float angle);
void RwMatrixScale(MATRIX4X4* mat, VECTOR* vecScale);

void DefinedState();

void SetRenderWareCamera(RwCamera* camera);

void rpWorldAddCamera(RwCamera* camera);
void rpWorldAddLight(RpLight* rpLight);
void rpWorldRemoveLight(RpLight* rpLight);

uint32_t GetTickCount();

RwTexture* LoadTextureFromTxd(const char* txdnname, const char* texturename);
};
