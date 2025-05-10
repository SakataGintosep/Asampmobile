#pragma once

#include "../game/cracked_classes.h"

typedef unsigned short VEHICLEID;

#define MAX_VEHICLES 2000
#define INVALID_VEHICLE_ID 0xFFFF

class CVehiclePool {
public:
	CVehicle* getAt(VEHICLEID vehicleID)
	{
		if (vehicleID < MAX_VEHICLES && m_slotState[vehicleID]) {
			return m_vehicles[vehicleID];
		}

		return nullptr;
	}

	bool getSlotState(VEHICLEID VehicleID)
	{
		if (VehicleID < MAX_VEHICLES) {
			return m_slotState[VehicleID];
		}

		return false;
	}

	VEHICLEID findIDFromGtaPtr(VEHICLE_TYPE* pGtaVehicle);
	uint32_t findGtaIDFromID(VEHICLEID VehicleID);

	VEHICLEID findNearestToLocalPlayerPed()
	{
		float fLeastDistance = 10000.0f;
		float fThisDistance;
		VEHICLEID ClosestSoFar = INVALID_VEHICLE_ID;

		VEHICLEID x = 0;
		while (x < MAX_VEHICLES) {
			if (m_slotState[x] && m_active[x]) {
				fThisDistance = m_vehicles[x]->getDistanceFromLocalPlayerPed();
				if (fThisDistance < fLeastDistance) {
					fLeastDistance = fThisDistance;
					ClosestSoFar = x;
				}
			}
			x++;
		}

		return ClosestSoFar;
	}

public:
	CVehicle* m_vehicles[MAX_VEHICLES];        // 0-8000
	VEHICLE_TYPE* m_gtaVehicles[MAX_VEHICLES]; // 8000-16000
	VECTOR m_position[MAX_VEHICLES];           // 16000-40000
	bool m_slotState[MAX_VEHICLES];            // 40000-42000
	bool m_active[MAX_VEHICLES];               // 42000-44000
	uint8_t m_pad[16012];                      // 44000-60012
};
static_assert(sizeof(CVehiclePool) == 60012, "sizeof(CVehiclePool) != 60012");
