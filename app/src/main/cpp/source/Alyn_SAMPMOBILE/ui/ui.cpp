#include "ui.h"
#include "../main.h"
#include "../samp.h"
#include "../voice/SpeakerList.h"
#include "../voice/MicroIcon.h"
#include "../voice/include/util/Render.h"

extern UI* pUI;

UI::UI(const ImVec2& display_size, const std::string& font_path)
		: Widget(), ImGuiWrapper(display_size, font_path)
{
	UISettings::Initialize(display_size);
	this->setFixedSize(display_size);
}

bool UI::initialize()
{
	if (!ImGuiWrapper::initialize()) {
		return false;
	}

	m_voicebutton = new VoiceButton();
	m_deathwindow = new DeathWindow();

	return true;
}

void UI::render()
{
	ImGuiWrapper::render();
}

void UI::shutdown()
{
	ImGuiWrapper::shutdown();
}

void UI::drawList()
{
	if (!visible()) {
		return;
	}

	if (m_deathwindow) {
		m_deathwindow->render(renderer());
	}

	draw(renderer());
}

void UI::touchEvent(const ImVec2& pos, TouchType type)
{
	Widget::touchEvent(pos, type);
}
