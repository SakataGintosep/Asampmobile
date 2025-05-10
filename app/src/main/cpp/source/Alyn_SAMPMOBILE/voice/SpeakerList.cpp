#include "../main.h"
#include "../ui/ui.h"
#include "../samp.h"

#include "SpeakerList.h"

#include "PluginConfig.h"

extern UI* pUI;

bool SpeakerList::Init() noexcept
{
	if (SpeakerList::initStatus) {
		return false;
	}

	try {
		SpeakerList::tSpeakerIcon = utils::LoadTextureFromTxd("*", "speaker");
	}
	catch (const std::exception& exception) {
		LogVoice("[sv:err:speakerlist:init] : failed to create speaker icon");
		SpeakerList::tSpeakerIcon = nullptr;
		return false;
	}

	if (!PluginConfig::IsSpeakerLoaded()) {
		PluginConfig::SetSpeakerLoaded(true);
	}

	SpeakerList::initStatus = true;

	return true;
}

void SpeakerList::Free() noexcept
{
	if (!SpeakerList::initStatus) {
		return;
	}

	SpeakerList::tSpeakerIcon = nullptr;

	SpeakerList::initStatus = false;
}

void SpeakerList::Show() noexcept
{
	if (!SpeakerList::initStatus) {
		return;
	}

	SpeakerList::showStatus = true;
}

bool SpeakerList::IsShowed() noexcept
{
	return SpeakerList::showStatus;
}

void SpeakerList::Hide() noexcept
{
	SpeakerList::showStatus = false;
}

void SpeakerList::Render()
{
	if (!SpeakerList::initStatus || !SpeakerList::IsShowed()) {
		return;
	}

	if (!SAMP::netgame()) {
		return;
	}

	CPlayerPool* pPlayerPool = SAMP::netgame()->m_pools->playerPool;
	if (!pPlayerPool) {
		return;
	}

	int curTextLine;
	char szText[64], szText2[64];
	ImVec2 textPos = ImVec2(25.0f, 500.0f);

	for (PLAYERID playerId{0}; playerId < MAX_PLAYERS; ++playerId) {
		CRemotePlayer* pPlayer = pPlayerPool->getAt(playerId);
		if (pPlayer && pPlayer->isActive()) {
			CPlayerPed* pPlayerPed = pPlayer->m_playerPed;
			if (pPlayerPed) {
				const auto playerName = pPlayerPool->getPlayerName(playerId);
				if (playerName) {
					if (!SpeakerList::playerStreams[playerId].empty()) {
						for (const auto& playerStream : SpeakerList::playerStreams[playerId]) {
							if (playerStream.second.GetType() == StreamType::LocalStreamAtPlayer) {
								VECTOR VecPos;

								if (!pPlayerPed->isAdded()) {
									continue;
								}
								VecPos.X = 0.0f;
								VecPos.Y = 0.0f;
								VecPos.Z = 0.0f;
								pPlayerPed->getBonePosition(8, &VecPos);

								if (pPlayerPed->getDistanceFromLocalPlayerPed() < 60.0f) {
									SpeakerList::Draw(&VecPos, pPlayerPed->getDistanceFromCamera());
								}
							}
						}

						if (curTextLine < 4 && playerName && strlen(playerName)) {
							ImVec2 a = ImVec2(textPos.x, textPos.y);
							ImVec2 b = ImVec2(textPos.x + UISettings::fontSize() / 2,
									textPos.y + UISettings::fontSize() / 2);
							ImGui::GetBackgroundDrawList()->AddImage(
									(ImTextureID) SpeakerList::tSpeakerIcon->raster, a, b);

							textPos.x = UISettings::fontSize();
							sprintf(szText, "%s (%d) ", playerName, playerId);
							pUI->renderer()->drawText(textPos, 0xFFFFFFFF, szText, true,
									UISettings::fontSize() / 2);

							for (const auto& streamInfo : SpeakerList::playerStreams[playerId]) {
								if (streamInfo.second.GetColor() == NULL) {
									continue;
								}

								textPos.x = ImGui::CalcTextSize(szText).x; // 1.6;
								sprintf(szText2, "[%s]", streamInfo.second.GetName().c_str());
								pUI->renderer()->drawText(textPos, streamInfo.second.GetColor(),
										szText2, true, UISettings::fontSize() / 2);
							}

							textPos.x -= ImGui::CalcTextSize(szText).x; // 1.7;
							textPos.y += UISettings::fontSize();

							curTextLine++;
						}
					}
				}
			}
		}
	}
}

void SpeakerList::Draw(VECTOR* vec, float fDist)
{
	VECTOR TagPos;

	TagPos.X = vec->X;
	TagPos.Y = vec->Y;
	TagPos.Z = vec->Z;
	TagPos.Z += 0.25f + (fDist * 0.0475f);

	VECTOR Out;
	// CSprite::CalcScreenCoors
	((void (*)(VECTOR*, VECTOR*, float*, float*, bool, bool)) (SA_Addr(0x5C5798 + 1)))(&TagPos,
			&Out, 0, 0, 0, 0);
	if (Out.Z < 1.0f) {
		return;
	}

	ImVec2 pos = ImVec2(Out.X, Out.Y);
	pos.x -= PluginConfig::kDefValSpeakerIconSize / 2;
	pos.y -= UISettings::fontSize();

	ImVec2 a = ImVec2(pos.x, pos.y);
	ImVec2 b = ImVec2(pos.x + PluginConfig::kDefValSpeakerIconSize,
			pos.y + PluginConfig::kDefValSpeakerIconSize);
	ImGui::GetBackgroundDrawList()->AddImage((ImTextureID) SpeakerList::tSpeakerIcon->raster, a, b);
}

void SpeakerList::OnSpeakerPlay(const Stream& stream, const uint16_t speaker) noexcept
{
	uint16_t wSpeaker = speaker;
	if (speaker < 0) {
		wSpeaker = 0;
	}
	else if (speaker > MAX_PLAYERS - 1) {
		wSpeaker = MAX_PLAYERS - 1;
	}
	if (speaker != wSpeaker) {
		return;
	}

	SpeakerList::playerStreams[speaker][(Stream*) (&stream)] = stream.GetInfo();
}

void SpeakerList::OnSpeakerStop(const Stream& stream, const uint16_t speaker) noexcept
{
	uint16_t wSpeaker = speaker;
	if (speaker < 0) {
		wSpeaker = 0;
	}
	else if (speaker > MAX_PLAYERS - 1) {
		wSpeaker = MAX_PLAYERS - 1;
	}
	if (speaker != wSpeaker) {
		return;
	}

	SpeakerList::playerStreams[speaker].erase((Stream*) (&stream));
}

std::array<std::unordered_map<Stream*, StreamInfo>, MAX_PLAYERS> SpeakerList::playerStreams;

bool SpeakerList::initStatus{false};
bool SpeakerList::showStatus{false};

RwTexture* SpeakerList::tSpeakerIcon{nullptr};