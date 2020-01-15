#pragma once

class Powerup
{
public:
	Powerup();
	Powerup(const char name[30], float health, float attack, float defense);
	~Powerup() {}

	void setName(const char name[30]);
	void setHealthMultiplier(float health) { m_healthMultiplier = health; }
	void setAttackMultiplier(float attack) { m_attackMultiplier = attack; }
	void setDefenseMultiplier(float defense) { m_defenseMultiplier = defense; }

	char* getName() { return m_name; }
	float getHealthMultiplier() { return m_healthMultiplier; }
	float getAttackMultiplier() { return m_attackMultiplier; }
	float getDefenseMultiplier() { return m_defenseMultiplier; }

	static bool compare(const Powerup* p1, const Powerup* p2);

private:
	char m_name[30];

	float m_healthMultiplier;
	float m_attackMultiplier;
	float m_defenseMultiplier;
};
