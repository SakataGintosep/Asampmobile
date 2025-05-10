#pragma once

// #define DEBUG_UI

#include "imguiwrapper.h"
#include "uisettings.h"

#include "widget.h"
#include "widgets/layout.h"
#include "widgets/label.h"
#include "widgets/button.h"
#include "widgets/image.h"
#include "widgets/progressbar.h"
#include "widgets/scrollpanel.h"

#include "samp_widgets/voicebutton.h"
#include "samp_widgets/deathwindow.h"

class UI : public Widget, public ImGuiWrapper {
public:
	UI(const ImVec2& display_size, const std::string& font_path);

	bool initialize() override;
	void render() override;
	void shutdown() override;

	VoiceButton* voicebutton() const { return m_voicebutton; }
	DeathWindow* deathwindow() const { return m_deathwindow; }

	static ImColor fixcolor(uint32_t color)
	{
		return ImColor(
				(int) ((color & 0xFF000000) >> 24),
				(int) ((color & 0x00FF0000) >> 16),
				(int) ((color & 0x0000FF00) >> 8));
	}

	float scaleX(float x)
	{
		return x * displaySize().x * (1.0f / 1920.0f);
	}

	float scaleY(float y)
	{
		return y * displaySize().y * (1.0f / 1080.0f);
	}

	virtual void touchEvent(const ImVec2& pos, TouchType type) override;

protected:
	void drawList() override;

private:
	VoiceButton* m_voicebutton;
	DeathWindow* m_deathwindow;
};
