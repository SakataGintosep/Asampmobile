#include "../main.h"
#include "../game/common.h"
#include "../game/utils.h"

#include "../ui/ui.h"

#include "MicroIcon.h"
#include "Header.h"
#include "PluginConfig.h"

extern UI* pUI;

bool MicroIcon::Init() noexcept
{
	if (MicroIcon::initStatus) {
		return false;
	}

	if (!PluginConfig::IsMicroLoaded()) {
		PluginConfig::SetMicroLoaded(true);
	}

	MicroIcon::initStatus = true;

	return true;
}

void MicroIcon::Free() noexcept
{
	if (!MicroIcon::initStatus) {
		return;
	}

	MicroIcon::initStatus = false;
}

void MicroIcon::Show() noexcept
{
	if (!MicroIcon::initStatus) {
		return;
	}

	MicroIcon::showStatus = true;

	if (pUI && pUI->voicebutton()) {
		pUI->voicebutton()->setVisible(true);
	}
}

bool MicroIcon::IsShowed() noexcept
{
	return MicroIcon::showStatus;
}

void MicroIcon::Hide() noexcept
{
	MicroIcon::showStatus = false;

	if (pUI && pUI->voicebutton()) {
		pUI->voicebutton()->setVisible(false);
	}
}

bool MicroIcon::initStatus{false};
bool MicroIcon::showStatus{false};
