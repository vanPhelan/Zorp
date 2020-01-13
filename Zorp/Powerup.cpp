#include "pch.h"
#include "Powerup.h"
#include <cstring>

Powerup::Powerup(const char name[30], float health, float attack, float defense) :
	m_healthMultiplier{ health },
	m_attackMultiplier{ attack },
	m_defenseMultiplier{ defense }
{
	strcpy_s(m_name, name);
}


Powerup::~Powerup()
{
}

char* Powerup::getName() {
	return m_name;
}

float Powerup::getHealthMultiplier()
{
	return m_healthMultiplier;
}

float Powerup::getAttackMultiplier()
{
	return m_attackMultiplier;
}

float Powerup::getDefenseMultiplier()
{
	return m_defenseMultiplier;
}
