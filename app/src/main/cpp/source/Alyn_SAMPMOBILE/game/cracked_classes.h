#pragma once

#include "utils.h"
#include "scripting.h"

// ui
class CrackedUI {
public:
	uint8_t m_pad[96];             // 0-96
	uintptr_t m_chat;              // 96-100
	char m_pad2[4];                // 100-104
	uintptr_t m_dialog;            // 104-108
	uintptr_t m_buttonpanel;       // 108-112
	uintptr_t m_extbuttonpanel;    // 112-116
	uintptr_t m_playertablist;     // 116-120
	char m_pad3[16];               // 120-136
};
static_assert(sizeof(CrackedUI) == 136, "sizeof(CrackedUI) != 136");

// entity
class CEntity {
public:
	CEntity() { };
	virtual ~CEntity() { };

	bool isAdded()
	{
		if (m_entity) {
			if (m_entity->vtable == SA_Addr(0x667D24)) { // CPlaceable
				return false;
			}

			if (m_entity->dwUnkModelRel) {
				return true;
			}
		}

		return false;
	}

	void add()
	{
		if (!m_entity || m_entity->vtable == SA_Addr(0x667D24)) { // CPlaceable
			return;
		}

		if (!m_entity->dwUnkModelRel) {

			VECTOR vec = {0.0f, 0.0f, 0.0f};

			setMoveSpeedVector(vec);
			setTurnSpeedVector(vec);

			// CWorld::Add
			((void (*)(ENTITY_TYPE*)) (SA_Addr(0x4233C8 + 1)))(m_entity);

			MATRIX4X4 mat;
			getMatrix(&mat);
			teleportTo(mat.pos.X, mat.pos.Y, mat.pos.Z);
		}
	}

	void remove()
	{
		if (!m_entity || m_entity->vtable == SA_Addr(0x667D24)) { // CPlaceable
			return;
		}

		if (m_entity->dwUnkModelRel) {
			// CWorld::Remove
			((void (*)(ENTITY_TYPE*)) (SA_Addr(0x4232BC + 1)))(m_entity);
		}
	}

	void getMatrix(MATRIX4X4* matrix)
	{
		if (!m_entity || !m_entity->mat) {
			return;
		}

		matrix->right.X = m_entity->mat->right.X;
		matrix->right.Y = m_entity->mat->right.Y;
		matrix->right.Z = m_entity->mat->right.Z;

		matrix->up.X = m_entity->mat->up.X;
		matrix->up.Y = m_entity->mat->up.Y;
		matrix->up.Z = m_entity->mat->up.Z;

		matrix->at.X = m_entity->mat->at.X;
		matrix->at.Y = m_entity->mat->at.Y;
		matrix->at.Z = m_entity->mat->at.Z;

		matrix->pos.X = m_entity->mat->pos.X;
		matrix->pos.Y = m_entity->mat->pos.Y;
		matrix->pos.Z = m_entity->mat->pos.Z;
	}

	void setMatrix(MATRIX4X4 matrix)
	{
		if (!m_entity || !m_entity->mat) {
			return;
		}

		m_entity->mat->right.X = matrix.right.X;
		m_entity->mat->right.Y = matrix.right.Y;
		m_entity->mat->right.Z = matrix.right.Z;

		m_entity->mat->up.X = matrix.up.X;
		m_entity->mat->up.Y = matrix.up.Y;
		m_entity->mat->up.Z = matrix.up.Z;

		m_entity->mat->at.X = matrix.at.X;
		m_entity->mat->at.Y = matrix.at.Y;
		m_entity->mat->at.Z = matrix.at.Z;

		m_entity->mat->pos.X = matrix.pos.X;
		m_entity->mat->pos.Y = matrix.pos.Y;
		m_entity->mat->pos.Z = matrix.pos.Z;
	}

	void getMoveSpeedVector(PVECTOR vector)
	{
		vector->X = m_entity->vecMoveSpeed.X;
		vector->Y = m_entity->vecMoveSpeed.Y;
		vector->Z = m_entity->vecMoveSpeed.Z;
	}

	void setMoveSpeedVector(VECTOR vector)
	{
		m_entity->vecMoveSpeed.X = vector.X;
		m_entity->vecMoveSpeed.Y = vector.Y;
		m_entity->vecMoveSpeed.Z = vector.Z;
	}

	void getTurnSpeedVector(PVECTOR vector)
	{
		vector->X = m_entity->vecTurnSpeed.X;
		vector->Y = m_entity->vecTurnSpeed.Y;
		vector->Z = m_entity->vecTurnSpeed.Z;
	}

	void setTurnSpeedVector(VECTOR vector)
	{
		m_entity->vecTurnSpeed.X = vector.X;
		m_entity->vecTurnSpeed.Y = vector.Y;
		m_entity->vecTurnSpeed.Z = vector.Z;
	}

	void teleportTo(float fX, float fY, float fZ)
	{
		MATRIX4X4 mat;

		if (m_entity && m_entity->vtable != SA_Addr(0x667D24)) /* CPlaceable */
		{
			uint16_t modelIndex = m_entity->nModelIndex;
			if (modelIndex != TRAIN_PASSENGER_LOCO && modelIndex != TRAIN_FREIGHT_LOCO && modelIndex != TRAIN_TRAM)
				//((void(*)(ENTITY_TYPE*, float, float, float, bool))(*(void**)(m_entity->vtable + 0x3C)))(m_entity, fX, fY, fZ, 0);
			{
				if (fX > 3000.0f || fX < -3000.0f || fY > 3000.0f || fY < -3000.0f) {
					getMatrix(&mat);
					mat.pos = VECTOR{fX, fY, fZ};
					setMatrix(mat);
					updateRwMatrixAndFrame();
				}
				else {
					((void (*)(ENTITY_TYPE*, float, float, float)) (*(void**) (m_entity->vtable + 0x3C)))(m_entity, fX, fY, fZ);
				}
			}
			else {
				scriptCommand(&put_train_at, m_gtaId, fX, fY, fZ);
			}
		}
	}

	void updateRwMatrixAndFrame()
	{
		if (m_entity && m_entity->vtable != SA_Addr(0x667D24)) // CPlaceable
		{
			if (m_entity->pdwRenderWare) {
				if (m_entity->mat) {
					uintptr_t pRwMatrix = *(uintptr_t*) (m_entity->pdwRenderWare + 4) + 0x10;

					// CMatrix::UpdateRwMatrix
					((void (*)(MATRIX4X4*, uintptr_t)) (SA_Addr(0x44EDEE + 1)))(m_entity->mat, pRwMatrix);

					// CEntity::UpdateRwFrame
					((void (*)(ENTITY_TYPE*)) (SA_Addr(0x3EBFE8 + 1)))(m_entity);
				}
			}
		}
	}

	float getDistanceFromLocalPlayerPed()
	{
		// sub_F8E1C
		return Memory::callFunction<float>(SAMP_Addr(0xF8E1C + 1), this);
	}

	float getDistanceFromCamera()
	{
		MATRIX4X4 matEnt;
		if (!m_entity || m_entity->vtable == SA_Addr(0x667D24)) { // CPlaceable
			return 100000.0f;
		}

		getMatrix(&matEnt);

		float tmpX = (matEnt.pos.X - *(float*) (SA_Addr(0x9528D4)));
		float tmpY = (matEnt.pos.Y - *(float*) (SA_Addr(0x9528D8)));
		float tmpZ = (matEnt.pos.Z - *(float*) (SA_Addr(0x9528DC)));

		return sqrt(tmpX * tmpX + tmpY * tmpY + tmpZ * tmpZ);
	}

public:
	ENTITY_TYPE* m_entity;
	uint32_t m_gtaId;
};
static_assert(sizeof(CEntity) == 12, "sizeof(CEntity) != 12");

// playerped
class CPlayerPed : public CEntity {
public:
	CPlayerPed()
	{
		// sub_103B88
		Memory::callFunction<CPlayerPed*>(SAMP_Addr(0x103B88 + 1), this);
	}

	void getBonePosition(int i, VECTOR* vec)
	{
		if (!m_ped || m_entity->vtable == SA_Addr(0x667D24)) { // CPlaceable
			return;
		}

		// CPed::GetBonePosition
		Memory::callFunction<void>(SA_Addr(0x4A4A9C + 1), m_ped, vec, i, 0);
	}

	VEHICLE_TYPE* getGtaVehicle()
	{
		return (VEHICLE_TYPE*) m_ped->pVehicle;
	}

	bool isInVehicle()
	{
		if (!m_ped) {
			return false;
		}

		if (IN_VEHICLE(m_ped)) {
			return true;
		}

		return false;
	}

	bool isAPassenger()
	{
		if (m_ped->pVehicle && IN_VEHICLE(m_ped)) {
			VEHICLE_TYPE* pVehicle = (VEHICLE_TYPE*) m_ped->pVehicle;

			if (pVehicle->pDriver != m_ped ||
					pVehicle->entity.nModelIndex == TRAIN_PASSENGER ||
					pVehicle->entity.nModelIndex == TRAIN_FREIGHT) {
				return true;
			}
			else {
				return false;
			}
		}

		return false;
	}

	bool isInJetpack()
	{
		if (!m_ped || !utils::GamePool_Ped_GetAt(m_gtaId)) {
			return false;
		}

		return scriptCommand(&is_actor_using_jetpack, m_playerNumber);
	}

public:
	// entity                // 0-12
	char m_pad[80];          // 12-92
	PED_TYPE* m_ped;         // 92-96
	char m_playerNumber;     // 96-97
	char m_pad2[655];        // 97-752
};
static_assert(sizeof(CPlayerPed) == 752, "sizeof(CPlayerPed) != 752");

// vehicle
class CVehicle : public CEntity {
public:
	// entity                // 0-12
	VEHICLE_TYPE* m_vehicle; // 12-16
	char m_pad[64];          // 16-80

	// new
	float m_rotation;        // 80-84
};
static_assert(sizeof(CVehicle) == 84, "sizeof(CVehicle) != 84");

// object
class CObject : public CEntity {
public:
	// entity            // 0-12
	char m_pad[1172];    // 12-1184
};
static_assert(sizeof(CObject) == 1184, "sizeof(CObject) != 1184");

// game
class CGame {
public:
	CPlayerPed* findPlayerPed()
	{
		if (!m_gamePlayer) {
			m_gamePlayer = new CPlayerPed();
		}
		return m_gamePlayer;
	}

public:
	char m_pad[8];            // 0-8
	CPlayerPed* m_gamePlayer; // 8-12
	char m_pad2[76];          // 12-88
};
static_assert(sizeof(CGame) == 88, "sizeof(CGame) != 88");
