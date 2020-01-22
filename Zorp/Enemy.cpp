#include "pch.h"
#include "Enemy.h"
#include "GameDefines.h"
#include <iostream>

Enemy::Enemy() : Character({ 0,0 }, 20, 10, 5)
{
	m_priority = PRIORITY_ENEMY;
}

void Enemy::onAttacked(int attackPoints)
{
	int damage = attackPoints - m_defensePoints;
	if (damage < 1) damage = 1;
	m_hitPoints -= damage;

	if (m_hitPoints < 0) m_hitPoints = 0;
}

void Enemy::draw()
{
	std::cout << RED << ICON_ENEMY << RESET_COLOR;
}

void Enemy::drawDescription()
{
	std::cout << INDENT << RED << "BEWARE." << RESET_COLOR <<
		" An enemy is approaching." << std::endl;
}

void Enemy::lookAt()
{
	std::cout << EXTRA_OUTPUT_POS << RESET_COLOR <<
		"LOOK OUT! An enemy is approaching." << std::endl;
}
