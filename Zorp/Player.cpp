#include "pch.h"
#include "Player.h"
#include "GameDefines.h"
#include <iostream>
#include <algorithm>

Player::Player() :
	m_mapPosition{ 0, 0 },
	m_healthPoints{ 100 },
	m_attackPoints{ 20 },
	m_defensePoints{ 20 }
{ }

Player::Player(int x, int y) :
	m_mapPosition{ x, y },
	m_healthPoints{ 100 },
	m_attackPoints{ 20 },
	m_defensePoints{ 20 }
{ }

Player::~Player() { }

void Player::setPosition(Point2D position)
{
	m_mapPosition = position;
}

Point2D Player::getPosition()
{
	return m_mapPosition;
}

void Player::draw()
{
	int x = INDENT_X + (6 * m_mapPosition.x) + 3;
	int y = MAP_Y + m_mapPosition.y;

	//Draw the player's position on the map
	//Move cursor to map pos and delete character at current position
	std::cout << CSI << y << ";" << x << "H";
	std::cout << MAGENTA << ICON_PLAYER << RESET_COLOR;

	std::cout << INVENTORY_OUTPUT_POS;
	for (std::vector<Powerup>::iterator it = m_powerups.begin(); it < m_powerups.end(); it++) {
		std::cout << (*it).getName() << "\t";
	}
}

bool Player::executeCommand(int command, Room* room)
{
	//Move player in input direction
	switch (command) {
	case EAST:
		if (m_mapPosition.x < MAZE_WIDTH - 1)
			m_mapPosition.x++;
		return true;
	case WEST:
		if (m_mapPosition.x > 0)
			m_mapPosition.x--;
		return true;
	case NORTH:
		if (m_mapPosition.y > 0)
			m_mapPosition.y--;
		return true;
	case SOUTH:
		if (m_mapPosition.y < MAZE_HEIGHT - 1)
			m_mapPosition.y++;
		return true;
	case PICKUP:
		return pickup(room);
	}
	return false;
}
/*
bool Player::pickup(int roomType)
{
	static const char descriptors[15][30] = {
		"indifference", "invisibility", "invulnerability", "incontinence",
		"improbability", "impatience", "indecision", "inspiration",
		"independence", "incurability", "integration", "invocation",
		"inferno", "indigestion", "inoculation"
	};

	int item = rand() % 15;

	char name[30];

	//Add the first part of the item name
	switch (roomType) {
	case TREASURE_HP:
		strcpy_s(name, "potion of ");
		break;
	case TREASURE_AT:
		strcpy_s(name, "sword of ");
		break;
	case TREASURE_DF:
		strcpy_s(name, "shield of ");
		break;
	default:
		return false;
	}

	//Add the second part of the item name
	strncat_s(name, descriptors[item], 30);

	std::cout << EXTRA_OUTPUT_POS << RESET_COLOR
		<< "You pick up the " << name << "." << std::endl;
	m_powerups.push_back(Powerup(name, 1, 1, 1));

	std::sort(m_powerups.begin(), m_powerups.end(), Powerup::compare);

	std::cout << INDENT << "Press 'Enter' to continue.";
	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());
	std::cin.get();
	return true;
}
*/

bool Player::pickup(Room* room)
{
	static const char itemNames[14][30] = {
		"beige", "black", "blue", "brown", "burgundy",
		"clear", "green", "grey", "pink", "plaid",
		"purple", "red", "white", "yellow"
	};

	int item = rand() % 14;

	char name[30];
	strcpy_s(name, 30, itemNames[item]);

	switch (room->getType()) {
	case TREASURE_HP:
		strncat_s(name, 30, " potion", 30);
		break;
	case TREASURE_AT:
		strncat_s(name, 30, " sword", 30);
		break;
	case TREASURE_DF:
		strncat_s(name, 30, " shield", 30);
		break;
	default:
		return false;
	}

	//Add the treasure to the inventory
	m_powerups.push_back(Powerup(name, 1, 1, 1.1f));
	std::cout << EXTRA_OUTPUT_POS << RESET_COLOR;
	std::cout << "You pick up the " << name << "." << std::endl;

	//Remove the treasure from the room
	room->setType(EMPTY);

	//Sort the inventory
	std::sort(m_powerups.begin(), m_powerups.end(), Powerup::compare);

	std::cout << INDENT << "Press 'Enter' to continue.";
	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());
	std::cin.get();
	return true;
}
