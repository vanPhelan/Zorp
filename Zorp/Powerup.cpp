#include "pch.h"
#include "Powerup.h"
#include "GameDefines.h"
#include <cstring>
#include <iostream>

Powerup::Powerup() :
	m_healthMultiplier{ 1 },
	m_attackMultiplier{ 1 },
	m_defenseMultiplier{ 1 }
{
	m_priority = PRIORITY_POWERUP;
	strcpy_s(m_name, "nondescript item");
}

Powerup::Powerup(const char name[30], float health, float attack, float defense) :
	m_healthMultiplier{ health },
	m_attackMultiplier{ attack },
	m_defenseMultiplier{ defense }
{
	m_priority = PRIORITY_POWERUP;
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
}

void Powerup::draw()
{
	std::cout << YELLOW << ICON_TREASURE << RESET_COLOR;
}

void Powerup::drawDescription()
{
	std::cout << INDENT << "There appears to be some treasure here. " <<
		"Perhaps you should investigate further." << std::endl;
}

void Powerup::lookAt()
{
	std::cout << EXTRA_OUTPUT_POS << RESET_COLOR <<
		"There is some treasure here. It looks small enough to pick up." << std::endl;
}
