#pragma once
#include "Point2D.h"

class Player;
class Enemy;
class Food;
class Powerup;

class Room
{
public:
	Room();
	~Room() {}

	void setPosition(Point2D position) { m_mapPosition = position; }
	void setType(int type) { m_type = type; }
	void setEnemy(Enemy* enemy) { m_enemy = enemy; }
	void setFood(Food* food) { m_food = food; }
	void setPowerup(Powerup* powerup) { m_powerup = powerup; }

	int getType() { return m_type; }
	Enemy* getEnemy() { return m_enemy; }
	Food* getFood() { return m_food; }
	Powerup* getPowerup() { return m_powerup; }

	void draw();
	void drawDescription();

private:
	Point2D m_mapPosition;
	int m_type;

	Enemy* m_enemy;
	Food* m_food;
	Powerup* m_powerup;
};

