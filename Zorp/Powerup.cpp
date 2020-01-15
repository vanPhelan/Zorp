#include "pch.h"
#include "Powerup.h"
#include <cstring>

Powerup::Powerup() :
	m_healthMultiplier{ 1 },
	m_attackMultiplier{ 1 },
	m_defenseMultiplier{ 1 }
{
	strcpy_s(m_name, "nondescript item");
}

Powerup::Powerup(const char name[30], float health, float attack, float defense) :
	m_healthMultiplier{ health },
	m_attackMultiplier{ attack },
	m_defenseMultiplier{ defense }
{
	strcpy_s(m_name, name);
}

void Powerup::setName(const char name[30])
{
	strcpy_s(m_name, name);
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
