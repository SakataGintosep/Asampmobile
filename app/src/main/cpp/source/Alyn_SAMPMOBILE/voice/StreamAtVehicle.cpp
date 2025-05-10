#include "../main.h"
#include "../samp.h"

#include "StreamAtVehicle.h"

#include "StreamInfo.h"

StreamAtVehicle::StreamAtVehicle(const uint16_t color, std::string name,
		const float distance, const VEHICLEID vehicleId) noexcept
		: LocalStream(StreamType::LocalStreamAtVehicle, color, std::move(name), distance),
		  vehicleId(vehicleId) { }

void StreamAtVehicle::Tick() noexcept
{
	this->LocalStream::Tick();

	if (!SAMP::netgame()) return;

	CVehiclePool* pVehiclePool = SAMP::netgame()->m_pools->vehiclePool;
	if (!pVehiclePool) return;

	CVehicle* pVehicle = pVehiclePool->getAt(this->vehicleId);
	if (!pVehicle) return;

	MATRIX4X4 pVehicleMatrix;
	pVehicle->getMatrix(&pVehicleMatrix);

	for (const auto& channel : this->GetChannels()) {
		if (channel->HasSpeaker()) {
			BASS_ChannelSet3DPosition(channel->GetHandle(),
					reinterpret_cast<BASS_3DVECTOR*>(&pVehicleMatrix.pos),
					nullptr, nullptr);
		}
	}
}

void StreamAtVehicle::OnChannelCreate(const Channel& channel) noexcept
{
	static const BASS_3DVECTOR kZeroVector{0, 0, 0};

	this->LocalStream::OnChannelCreate(channel);

	if (!SAMP::netgame()) return;

	CVehiclePool* pVehiclePool = SAMP::netgame()->m_pools->vehiclePool;
	if (!pVehiclePool) return;

	CVehicle* pVehicle = pVehiclePool->getAt(this->vehicleId);
	if (!pVehicle) return;

	MATRIX4X4 pVehicleMatrix;
	pVehicle->getMatrix(&pVehicleMatrix);

	BASS_ChannelSet3DPosition(channel.GetHandle(),
			reinterpret_cast<BASS_3DVECTOR*>(&pVehicleMatrix.pos),
			&kZeroVector, &kZeroVector);
}
