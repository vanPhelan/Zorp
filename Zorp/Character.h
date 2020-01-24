#pragma once
#include "GameObject.h"
#include <vector>

class Powerup;

class Character : public GameObject
{
public:
	Character();
	Character(Point2D position, int health, int attack, int defense);
	~Character();

	void addPowerup(Powerup* powerup);

	virtual void draw() = 0;
	virtual void drawDescription() = 0;
	virtual void lookAt() = 0;

	void save(std::ofstream& out);

	int getHP() { return m_hitPoints; }
	int getAP() { return m_attackPoints; }
	int getDP() { return m_defensePoints; }

	bool isAlive() { return m_hitPoints > 0; }

protected:
	std::vector<Powerup*> m_powerups;

	int m_hitPoints;
	int m_attackPoints;
	int m_defensePoints;
};

