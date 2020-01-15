#pragma once
#include "Point2D.h"
#include <vector>

class Room;
class Enemy;
class Powerup;

class Player
{
public:
	Player();
	Player(int x, int y);
	~Player() {}

	void addPowerup(Powerup* powerup);

	void setPosition(const Point2D& position) { m_mapPosition = position; }

	Point2D getPosition() { return m_mapPosition; }

	void draw();

	void executeCommand(int command, Room* room);

private:
	void pickup(Room* room);
	void attack(Enemy* enemy);

private:
	Point2D m_mapPosition;

	std::vector<Powerup*> m_powerups;

	int m_hitPoints;
	int m_attackPoints;
	int m_defensePoints;
};

