#include <imgui/imgui.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui_internal.h>

#include <string>
#include "uisettings.h"
#include "../settings.h"

/* scaling */
ImVec2 UISettings::m_baseSize = ImVec2(640.0f, 480.0f);
ImVec2 UISettings::m_scaleFactor = ImVec2(1.0f, 1.0f);

/* font */
float UISettings::m_fontSize = 24.0f;
float UISettings::m_outlineSize = 2.0f;

/* widgets */
float UISettings::m_padding = 15.0f;

/* splashscreen */
ImVec2 UISettings::m_splashscreenLogoPos = ImVec2(0.0f, 0.0f);
ImVec2 UISettings::m_splashscreenLogoSize = ImVec2(130.0f, 180.0f);
ImVec2 UISettings::m_splashScreenPBarPos = ImVec2(100.0f, 430.0f);
ImVec2 UISettings::m_splashScreenPBarSize = ImVec2(440.0f, 10.0f);

/* buttonpanel */
ImVec2 UISettings::m_buttonPanelPos = ImVec2(5.0f, 165.0f);
ImVec2 UISettings::m_buttonPanelSize = ImVec2(480.0f, 50.0f);

/* voice button */
ImVec2 UISettings::m_voiceButtonPos = ImVec2(460.0f, 280.0f);
ImVec2 UISettings::m_voiceButtonSize = ImVec2(45.0f, 65.0f);

/* button colors */
ImColor UISettings::m_buttonColor = ImColor(0.11f, 0.11f, 0.11f, 0.80f);
ImColor UISettings::m_buttonFocusedColor = ImColor(0.1f, 0.1f, 0.7f, 1.0f);

void UISettings::Initialize(const ImVec2& display_size)
{
	m_scaleFactor = display_size / m_baseSize;

	/* font */
	m_fontSize = Settings::fontsize();
	m_fontSize *= m_scaleFactor.y;

	/* splashscreen */
	m_splashscreenLogoSize = m_splashscreenLogoSize * m_scaleFactor;
	m_splashscreenLogoPos = (display_size * 0.5f) - (m_splashscreenLogoSize * 0.5f);
	m_splashScreenPBarPos = m_splashScreenPBarPos * m_scaleFactor;
	m_splashScreenPBarSize = m_splashScreenPBarSize * m_scaleFactor;

	/* buttonpanel */
	m_buttonPanelPos = m_buttonPanelPos * m_scaleFactor;
	m_buttonPanelSize = m_buttonPanelSize * m_scaleFactor;

	/* button voice */
	m_voiceButtonPos = m_voiceButtonPos * m_scaleFactor;
	m_voiceButtonSize = m_voiceButtonSize * m_scaleFactor;
}
