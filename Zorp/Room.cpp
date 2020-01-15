#include "pch.h"
#include "Room.h"
#include "Powerup.h"
#include "Player.h"
#include "GameDefines.h"
#include <iostream>

static const char descriptors[15][30] = {
		"indifference", "invisibility", "invulnerability", "incontinence",
		"improbability", "impatience", "indecision", "inspiration",
		"independence", "incurability", "integration", "invocation",
		"inferno", "indigestion", "inoculation"
};

Room::Room() : m_type{ EMPTY }, m_mapPosition{ 0, 0 }, m_powerup{ nullptr }
{
}

Room::~Room()
{
	if (m_powerup != nullptr)
		delete m_powerup;
}

void Room::setPosition(Point2D position)
{
	m_mapPosition = position;
}

void Room::setType(int type)
{
	m_type = type;

	//Stop if type is not a treasure
	if (!(m_type == TREASURE_HP || m_type == TREASURE_AT || m_type == TREASURE_DF))
		return;
	//Stop if the treasure is empty
	if (m_powerup != nullptr)
		return;

	int item = rand() % 15;

	char name[30] = "";

	float hp = 1;
	float at = 1;
	float df = 1;

	//Add the first part of the item name
	switch (type) {
	case TREASURE_HP:
		strcpy_s(name, "potion of ");
		hp = 1.1f;
		break;
	case TREASURE_AT:
		strcpy_s(name, "sword of ");
		at = 1.1f;
		break;
	case TREASURE_DF:
		strcpy_s(name, "shield of ");
		df = 1.1f;
		break;
	}

	//Add the second part of the item name
	strncat_s(name, descriptors[item], 30);
	if (m_powerup != nullptr)
		delete m_powerup;
	m_powerup = new Powerup(name, hp, at, df);
}

int Room::getType()
{
	return m_type;
}

void Room::draw()
{
	//Find the console output position
	int outX = INDENT_X + (6 * m_mapPosition.x) + 1;
	int outY = MAP_Y + m_mapPosition.y;

	//Jump to the correct location
	std::cout << CSI << outY << ";" << outX << "H";
	//Draw the room
	std::cout << "[ ";
	switch (m_type) {
	case EMPTY:
		std::cout << GREEN << ICON_EMPTY;
		break;
	case ENEMY:
		std::cout << RED << ICON_ENEMY;
		break;
	case TREASURE:
	case TREASURE_HP:
	case TREASURE_AT:
	case TREASURE_DF:
		std::cout << YELLOW << ICON_TREASURE;
		break;
	case FOOD:
		std::cout << CYAN << ICON_FOOD;
		break;
	case ENTRANCE:
		std::cout << WHITE << ICON_ENTRANCE;
		break;
	case EXIT:
		std::cout << WHITE << ICON_EXIT;
		break;
	}
	std::cout << RESET_COLOR << " ] ";
}

void Room::drawDescription()
{
	//Reset the draw color
	std::cout << RESET_COLOR;
	//Reposition cursor
	std::cout << CSI << ROOM_DESC_Y << ";" << 0 << "H";
	//Delete and insert 4 lines
	std::cout << CSI << "4M" << CSI << "4L" << std::endl;

	//Write description of current room
	switch (m_type) {
	case EMPTY:
		std::cout << INDENT << "You are in an empty meadow. There is nothing of note here."
			<< std::endl;
		break;
	case ENEMY:
		std::cout << INDENT << RED << "BEWARE." << RESET_COLOR << " An enemy is approaching."
			<< std::endl;
		break;
	case TREASURE:
	case TREASURE_HP:
	case TREASURE_AT:
	case TREASURE_DF:
		std::cout << INDENT << "There appears to be some treasure here! Perhaps you should investigate further."
			<< std::endl;
		break;
	case FOOD:
		std::cout << INDENT << "At last! You collect some food to sustain you on your journey."
			<< std::endl;
		break;
	case ENTRANCE:
		std::cout << INDENT << "The entrance you used to enter this maze is blocked. There is no going back."
			<< std::endl;
		break;
	case EXIT:
		std::cout << INDENT << "Despite all odds, you made it to the exit. Congratulations."
			<< std::endl;
	}
}

bool Room::executeCommand(int command, Player* player)
{
	switch (command) {
	case LOOK:
		std::cout << EXTRA_OUTPUT_POS << RESET_COLOR;
		switch (m_type) {
		case TREASURE_HP:
		case TREASURE_AT:
		case TREASURE_DF:
			std::cout << "There is some treasure here. It looks small enough to pick up." << std::endl;
			break;
		default:
			std::cout << "You look around but see nothing worth mentioning." << std::endl;
			break;
		}
		std::cout << INDENT << "Press 'Enter' to continue.";
		std::cin.clear();
		std::cin.ignore(std::cin.rdbuf()->in_avail());
		std::cin.get();
		return true;
	case FIGHT:
		std::cout << EXTRA_OUTPUT_POS << RESET_COLOR;
		std::cout << "You could try to fight, but you don't have a weapon." << std::endl;
		std::cout << INDENT << "Press 'Enter' to continue.";
		std::cin.clear();
		std::cin.ignore(std::cin.rdbuf()->in_avail());
		std::cin.get();
		return true;
	case PICKUP:
		return pickup(player);
	default:
		std::cout << EXTRA_OUTPUT_POS << RESET_COLOR;
		std::cout << "You try, but you just can't do it." << std::endl;
		std::cout << INDENT << "Press 'Enter' to continue.";
		std::cin.clear();
		std::cin.ignore(std::cin.rdbuf()->in_avail());
		std::cin.get();
		break;
	}
	return false;
}

bool Room::pickup(Player* player)
{
	//Ensure there is a powerup
	if (m_powerup == nullptr) {
		std::cout << EXTRA_OUTPUT_POS << RESET_COLOR
			<< "There is nothing here to pick up." << std::endl;
		return true;
	}

	//Add powerup to player's inventory
	std::cout << EXTRA_OUTPUT_POS << RESET_COLOR
		<< "You pick up the " << m_powerup->getName() << "." << std::endl;
	player->addPowerup(m_powerup);

	//Remove powerup from the room
	m_powerup = nullptr;
	m_type = EMPTY;

	std::cout << INDENT << "Press 'Enter' to continue.";
	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());
	std::cin.get();
	return true;
}
