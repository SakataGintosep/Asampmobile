#pragma once

#include "../../java.h"

class VoiceButton {
public:
	VoiceButton()
	{
		m_visible = false;
		m_recording = false;
	}

	void setVisible(bool visible)
	{
		m_visible = visible;
		g_java->showVoice(visible);

		if (!visible && recording()) {
			setRecording(false);
		}
	}

	bool visible() const { return m_visible; }

	void setRecording(bool recording) { m_recording = recording; }
	bool recording() const { return m_recording; }

private:
	bool m_visible;
	bool m_recording;
};
