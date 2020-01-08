#include "pch.h"
#include "Player.h"
#include "GameDefines.h"
#include <iostream>

Player::Player()
{
	m_mapPosition = { 0, 0 };
}

Player::Player(int x, int y) {
	m_mapPosition = { x, y };
}

Player::~Player()
{
}

void Player::setPosition(Point2D position) {
	m_mapPosition = position;
}

Point2D Player::getPosition() {
	return m_mapPosition;
}

void Player::draw() {
	int x = INDENT_X + (6 * m_mapPosition.x) + 3;
	int y = MAP_Y + m_mapPosition.y;

	//Draw the player's position on the map
	//Move cursor to map pos and delete character at current position
	std::cout << CSI << y << ";" << x << "H";
	std::cout << MAGENTA << ICON_PLAYER << RESET_COLOR;
}

bool Player::executeCommand(int command) {
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
	}
	return false;
}