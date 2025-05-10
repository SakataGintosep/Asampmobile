#include "SampVoice.h"
#include "../../Header.h"

bool SampVoice::Init() noexcept
{
	if (SampVoice::initStatus) {
		return false;
	}

	LogVoice("[dbg:samp:init] : module initializing...");

	SampVoice::loadCallbacks.clear();
	SampVoice::exitCallbacks.clear();

	SampVoice::loadStatus = false;

	LogVoice("[dbg:samp:init] : module initialized");

	SampVoice::initStatus = true;

	return true;
}

bool SampVoice::IsInited() noexcept
{
	return SampVoice::initStatus;
}

bool SampVoice::IsLoaded() noexcept
{
	return SampVoice::loadStatus;
}

void SampVoice::Free() noexcept
{
	if (!SampVoice::initStatus) {
		return;
	}

	LogVoice("[dbg:samp:free] : module releasing...");

	if (SampVoice::loadStatus) {
		for (const auto& exitCallback : SampVoice::exitCallbacks) {
			if (exitCallback != nullptr) {
				exitCallback();
			}
		}
	}

	SampVoice::loadStatus = false;

	SampVoice::loadCallbacks.clear();
	SampVoice::exitCallbacks.clear();

	LogVoice("[dbg:samp:free] : module released");

	SampVoice::initStatus = false;
}

std::size_t SampVoice::AddLoadCallback(LoadCallback callback) noexcept
{
	if (!SampVoice::initStatus) return -1;

	for (std::size_t i{0}; i < SampVoice::loadCallbacks.size(); ++i) {
		if (SampVoice::loadCallbacks[i] == nullptr) {
			SampVoice::loadCallbacks[i] = std::move(callback);
			return i;
		}
	}

	SampVoice::loadCallbacks.emplace_back(std::move(callback));
	return SampVoice::loadCallbacks.size() - 1;
}

std::size_t SampVoice::AddExitCallback(ExitCallback callback) noexcept
{
	if (!SampVoice::initStatus) return -1;

	for (std::size_t i{0}; i < SampVoice::exitCallbacks.size(); ++i) {
		if (SampVoice::exitCallbacks[i] == nullptr) {
			SampVoice::exitCallbacks[i] = std::move(callback);
			return i;
		}
	}

	SampVoice::exitCallbacks.emplace_back(std::move(callback));
	return SampVoice::exitCallbacks.size() - 1;
}

void SampVoice::RemoveLoadCallback(const std::size_t callback) noexcept
{
	if (!SampVoice::initStatus) return;

	if (callback >= SampVoice::loadCallbacks.size()) {
		return;
	}

	SampVoice::loadCallbacks[callback] = nullptr;
}

void SampVoice::RemoveExitCallback(const std::size_t callback) noexcept
{
	if (!SampVoice::initStatus) return;

	if (callback >= SampVoice::exitCallbacks.size()) {
		return;
	}

	SampVoice::exitCallbacks[callback] = nullptr;
}

bool SampVoice::initStatus{false};
bool SampVoice::loadStatus{false};

std::vector<SampVoice::LoadCallback> SampVoice::loadCallbacks;
std::vector<SampVoice::ExitCallback> SampVoice::exitCallbacks;