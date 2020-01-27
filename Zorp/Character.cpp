#include "pch.h"
#include "Character.h"
#include "Powerup.h"
#include "Game.h"
#include <algorithm>
#include <string>

Character::Character() :
	GameObject{ {0,0} },
	m_hitPoints{ 0 },
	m_attackPoints{ 0 },
	m_defensePoints{ 0 }
{
}

Character::Character(Point2D position, int health, int attack, int defense) :
	GameObject{ position },
	m_hitPoints{ health },
	m_attackPoints{ attack },
	m_defensePoints{ defense }
{
}

Character::~Character()
{
}

void Character::addPowerup(Powerup* powerup)
{
	//Add the powerup to the inventory
	m_powerups.push_back(powerup);
	//Sort the inventory
	std::sort(m_powerups.begin(), m_powerups.end(), Powerup::compare);
}

void Character::save(std::ofstream & out)
{
	if (!out.is_open())
		return;

	out.write((char*)&m_priority, sizeof(int));
	out.write((char*)&m_mapPosition, sizeof(Point2D));
	out.write((char*)&m_hitPoints, sizeof(int));
	out.write((char*)&m_attackPoints, sizeof(int));
	out.write((char*)&m_defensePoints, sizeof(int));

	int count = m_powerups.size();
	out.write((char*)&count, sizeof(int));
	for (int i = 0; i < m_powerups.size(); i++) {
		//Save only the name of each powerup
		//Each powerup will be save by the Game class
		//The character only needs a pointer to each
		out.write(m_powerups[i]->getName(), 30);
	}
}

bool Character::load(std::ifstream& in, const Game* game)
{
	if (!in.is_open())
		return false;

	//Load priority
	in.read((char*)&m_priority, sizeof(int));
	if (in.rdstate()) return false;

	//Load map position
	in.read((char*)&m_mapPosition, sizeof(Point2D));
	if (in.rdstate()) return false;

	//Load hit points
	in.read((char*)&m_hitPoints, sizeof(int));
	if (in.rdstate()) return false;

	//Load attack points
	in.read((char*)&m_attackPoints, sizeof(int));
	if (in.rdstate()) return false;

	//Load defense points
	in.read((char*)&m_defensePoints, sizeof(int));
	if (in.rdstate()) return false;

	//Load powerup count
	int powerupCount;
	in.read((char*)&powerupCount, sizeof(int));
	if (in.rdstate()) return false;

	char name[30] = { 0 };
	for (int i = 0; i < powerupCount; i++) {
		name[0] = 0;
		in.read(name, 30);
		if (in.rdstate()) return false;

		//Search for the powerup by name
		Powerup* powerup = game->findPowerup(name, true);
		m_powerups.push_back(powerup);
	}

	return true;
}
