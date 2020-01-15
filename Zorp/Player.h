#pragma once
#include "Point2D.h"
#include "Powerup.h"
#include <vector>

class Player
{
public:
	Player();
	Player(int x, int y);
	~Player();

	void addPowerup(Powerup* powerup);

	void setPosition(Point2D position);

	Point2D getPosition();

	void draw();

	bool executeCommand(int command);

private:
	Point2D m_mapPosition;

	std::vector<Powerup*> m_powerups;

	int m_healthPoints;
	int m_attackPoints;
	int m_defensePoints;
};

