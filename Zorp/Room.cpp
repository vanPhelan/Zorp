#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "Enemy.h"
#include "Food.h"
#include "Powerup.h"
#include "GameDefines.h"
#include <iostream>

Room::Room() :
	m_type{ EMPTY },
	m_mapPosition{ 0, 0 },
	m_enemy{ nullptr },
	m_food{ nullptr },
	m_powerup{ nullptr }
{
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
		if (m_enemy != nullptr) {
			std::cout << RED << ICON_ENEMY;
		}
		else if (m_powerup != nullptr) {
			std::cout << YELLOW << ICON_TREASURE;
		}
		else if (m_food != nullptr) {
			std::cout << CYAN << ICON_FOOD;
		}
		else {
			std::cout << GREEN << ICON_EMPTY;
		}
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
		if (m_enemy != nullptr) {
			std::cout << INDENT << RED << "BEWARE." << RESET_COLOR << " An enemy is approaching."
				<< std::endl;
		}
		else if (m_powerup != nullptr) {
			std::cout << INDENT << "There appears to be some treasure here! Perhaps you should investigate further."
				<< std::endl;
		}
		else if (m_food != nullptr) {
			std::cout << INDENT << "At last! You find some food to sustain you on your journey."
				<< std::endl;
		}
		else {
			std::cout << INDENT << "You are in an empty meadow. There is nothing of note here."
				<< std::endl;
		}
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
