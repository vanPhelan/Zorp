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

	out << m_priority << ",";
	out << m_mapPosition.x << ",";
	out << m_mapPosition.y << ",";
	out << m_hitPoints << ",";
	out << m_attackPoints << ",";
	out << m_defensePoints << ",";

	out << m_powerups.size() << std::endl;
	for (int i = 0; i < m_powerups.size(); i++) {
		//Save only the name of each powerup
		//Each powerup will be save by the Game class
		//The character only needs a pointer to each
		out << m_powerups[i]->getName() << std::endl;
	}
}

bool Character::load(std::ifstream& in, const Game* game)
{
	if (!in.is_open())
		return false;

	char buffer[50] = { 0 };

	//Load priority
	in.get(buffer, 50, ',');
	if (in.rdstate() || buffer[0] == 0)
		return false;
	m_priority = std::stoi(buffer);

	//Load map position x
	in.ignore(1);
	in.get(buffer, 50, ',');
	if (in.rdstate() || buffer[0] == 0)
		return false;
	m_mapPosition.x = std::stoi(buffer);

	//Load map position y
	in.ignore(1);
	in.get(buffer, 50, ',');
	if (in.rdstate() || buffer[0] == 0)
		return false;
	m_mapPosition.y = std::stoi(buffer);

	//Load hit points
	in.ignore(1);
	in.get(buffer, 50, ',');
	if (in.rdstate() || buffer[0] == 0)
		return false;
	m_hitPoints = std::stoi(buffer);

	//Load attack points
	in.ignore(1);
	in.get(buffer, 50, ',');
	if (in.rdstate() || buffer[0] == 0)
		return false;
	m_attackPoints = std::stoi(buffer);

	//Load defense points
	in.ignore(1);
	in.get(buffer, 50, ',');
	if (in.rdstate() || buffer[0] == 0)
		return false;
	m_defensePoints = std::stoi(buffer);

	//Load powerup count
	in.ignore(1);
	in.getline(buffer, 50);
	if (in.rdstate() || buffer[0] == 0)
		return false;
	int powerupCount = std::stoi(buffer);

	for (int i = 0; i < powerupCount; i++) {
		char name[30] = { 0 };
		in.getline(name, 30);
		if (in.rdstate() || name[0] == 0)
			return false;

		//Search for the powerup by name
		Powerup* powerup = game->findPowerup(name, true);
		m_powerups.push_back(powerup);
	}

	return true;
}
