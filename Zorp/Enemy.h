#pragma once
#include "Point2D.h"

class Enemy
{
public:
	Enemy() : m_hitPoints{ 20 }, m_attackPoints{ 10 }, m_defensePoints{ 5 } {}
	~Enemy() {}

	void setPosition(const Point2D& position) { m_mapPosition = position; }
	Point2D getPosition() { return m_mapPosition; }

	int getHP() { return m_hitPoints; }
	int getAP() { return m_attackPoints; }
	int getDP() { return m_defensePoints; }

	bool isAlive() { return (m_hitPoints > 0); }

	void onAttacked(int attackPoints);

private:
	Point2D m_mapPosition;

	int m_hitPoints;
	int m_attackPoints;
	int m_defensePoints;
};

