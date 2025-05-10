#include "../main.h"
#include "../samp.h"

#include "StreamAtPlayer.h"

#include "StreamInfo.h"

StreamAtPlayer::StreamAtPlayer(const uint32_t color, std::string name,
		const float distance, const PLAYERID playerId) noexcept
		: LocalStream(StreamType::LocalStreamAtPlayer, color, std::move(name), distance),
		  playerId(playerId) { }

void StreamAtPlayer::Tick() noexcept
{
	this->LocalStream::Tick();

	if (!SAMP::netgame()) return;

	CPlayerPool* pPlayerPool = SAMP::netgame()->m_pools->playerPool;
	if (!pPlayerPool) return;

	CRemotePlayer* pPlayer = pPlayerPool->getAt(this->playerId);
	if (!pPlayer) return;

	CPlayerPed* pPlayerPed = pPlayer->m_playerPed;
	if (!pPlayerPed) return;

	MATRIX4X4 pPlayerMatrix;
	pPlayerPed->getMatrix(&pPlayerMatrix);

	for (const auto& channel : this->GetChannels()) {
		if (channel->HasSpeaker()) {
			BASS_ChannelSet3DPosition(channel->GetHandle(),
					reinterpret_cast<BASS_3DVECTOR*>(&pPlayerMatrix.pos),
					nullptr, nullptr);
		}
	}
}

void StreamAtPlayer::OnChannelCreate(const Channel& channel) noexcept
{
	static const BASS_3DVECTOR kZeroVector{0, 0, 0};

	this->LocalStream::OnChannelCreate(channel);

	if (!SAMP::netgame()) return;

	CPlayerPool* pPlayerPool = SAMP::netgame()->m_pools->playerPool;
	if (!pPlayerPool) return;

	CRemotePlayer* pPlayer = pPlayerPool->getAt(this->playerId);
	if (!pPlayer) return;

	CPlayerPed* pPlayerPed = pPlayer->m_playerPed;
	if (!pPlayerPed) return;

	MATRIX4X4 pPlayerMatrix;
	pPlayerPed->getMatrix(&pPlayerMatrix);

	BASS_ChannelSet3DPosition(channel.GetHandle(),
			reinterpret_cast<BASS_3DVECTOR*>(&pPlayerMatrix.pos),
			&kZeroVector, &kZeroVector);
}
