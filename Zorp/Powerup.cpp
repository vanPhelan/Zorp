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

bool Powerup::compare(const Powerup* p1, const Powerup* p2)
{
	if (strcmp(p1->m_name, p2->m_name) < 0) {
		return true;
	}
	else {
		return false;
	}

	//return (strcmp(p1->m_name, p2->m_name) < 0) ? true : false;
}
