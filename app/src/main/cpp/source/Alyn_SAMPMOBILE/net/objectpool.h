#pragma once

#include "../game/cracked_classes.h"

typedef unsigned short OBJECTID;

#define MAX_OBJECTS 1000

class CObjectPool {
public:
	bool getSlotState(OBJECTID objectId)
	{
		if (objectId > MAX_OBJECTS) {
			return false;
		}
		return m_slotState[objectId];
	};

	CObject* getAt(OBJECTID objectId)
	{
		if (objectId > MAX_OBJECTS) {
			return nullptr;
		}
		return m_objects[objectId];
	}

public:
	char m_pad[4];                   // 0-4
	bool m_slotState[MAX_OBJECTS];   // 4-1004
	CObject* m_objects[MAX_OBJECTS]; // 1004-5004
};
static_assert(sizeof(CObjectPool) == 5004, "sizeof(CObjectPool) != 5004");
