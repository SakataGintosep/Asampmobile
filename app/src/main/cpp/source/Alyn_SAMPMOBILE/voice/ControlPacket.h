#pragma once

#include "include/util/Memory.hpp"

#pragma pack(push, 1)

struct ControlPacket {
	uint16_t packet;
	uint16_t length;
	uint8_t data[];

	uint32_t GetFullSize();
};

#pragma pack(pop)

using ControlPacketContainer = memory::ObjectContainer<ControlPacket>;
using ControlPacketContainerPtr = memory::ObjectContainerPtr<ControlPacket>;
#define MakeControlPacketContainer MakeObjectContainer(ControlPacket)
