#include "../main.h"
#include "../samp.h"

#include "StreamAtObject.h"

#include "StreamInfo.h"

StreamAtObject::StreamAtObject(const uint32_t color, std::string name,
		const float distance, const WORD objectId) noexcept
		: LocalStream(StreamType::LocalStreamAtObject, color, std::move(name), distance),
		  objectId(objectId) { }

void StreamAtObject::Tick() noexcept
{
	this->LocalStream::Tick();

	if (!SAMP::netgame()) return;

	CObjectPool* pObjectPool = SAMP::netgame()->m_pools->objectPool;
	if (!pObjectPool) return;

	CObject* pObject = pObjectPool->getAt(this->objectId);
	if (!pObject) return;

	MATRIX4X4 pObjectMatrix;
	pObject->getMatrix(&pObjectMatrix);

	for (const auto& channel : this->GetChannels()) {
		if (channel->HasSpeaker()) {
			BASS_ChannelSet3DPosition(channel->GetHandle(),
					reinterpret_cast<BASS_3DVECTOR*>(&pObjectMatrix.pos),
					nullptr, nullptr);
		}
	}
}

void StreamAtObject::OnChannelCreate(const Channel& channel) noexcept
{
	static const BASS_3DVECTOR kZeroVector{0, 0, 0};

	this->LocalStream::OnChannelCreate(channel);

	if (!SAMP::netgame()) return;

	CObjectPool* pObjectPool = SAMP::netgame()->m_pools->objectPool;
	if (!pObjectPool) return;

	CObject* pObject = pObjectPool->getAt(this->objectId);
	if (!pObject) return;

	MATRIX4X4 pObjectMatrix;
	pObject->getMatrix(&pObjectMatrix);

	BASS_ChannelSet3DPosition(channel.GetHandle(),
			reinterpret_cast<BASS_3DVECTOR*>(&pObjectMatrix.pos),
			&kZeroVector, &kZeroVector);
}
