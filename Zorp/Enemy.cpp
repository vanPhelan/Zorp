#include "pch.h"
#include "Enemy.h"


void Enemy::onAttacked(int attackPoints)
{
	int damage = attackPoints - m_defensePoints;
	if (damage < 0) damage = 0;
	m_hitPoints -= damage;

	if (m_hitPoints < 0) m_hitPoints = 0;
}
