#pragma once
#include "Point2D.h"
#include <vector>

class GameObject;
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

	int getType() { return m_type; }
	Enemy* getEnemy();
	Food* getFood();
	Powerup* getPowerup();

	void addGameObject(GameObject* object);
	void removeGameObject(GameObject* object);
	void clearGameObjects();

	void draw();
	void drawDescription();
	void lookAt();

private:
	Point2D m_mapPosition;
	int m_type;

	std::vector <GameObject*> m_objects;
};

