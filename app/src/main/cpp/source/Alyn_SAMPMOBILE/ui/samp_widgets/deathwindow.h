#pragma once

#include <list>

struct DeathWindowStruct {
	std::string playerName;
	unsigned int playercolor;
	std::string killerName;
	unsigned int killercolor;
	uint8_t reason;
};

class DeathWindow {
public:
	DeathWindow();

	void record(const char* playername, unsigned int playercolor, const char* killername, unsigned int killercolor, uint8_t reason);

	void render(ImGuiRenderer* renderer);

	void setVisible(bool visible) { m_visible = visible; }
	bool visible() { return m_visible; }

public:
	const char* spriteIDForWeapon(uint8_t byteWeaponID);

private:
	std::list<DeathWindowStruct*> m_pDeathWindow;

	bool m_visible;
};
