#include "pch.h"
#include "Player.h"
#include "Room.h"
#include "Enemy.h"
#include "Food.h"
#include "Powerup.h"
#include "GameDefines.h"
#include <iostream>
#include <algorithm>

Player::Player() : Character({ 0,0 }, 100, 20, 20)
{
	m_priority = PRIORITY_PLAYER;
}

Player::Player(int x, int y) : Character({ x,y }, 100, 20, 20)
{
	m_priority = PRIORITY_PLAYER;
}

void Player::executeCommand(int command, Room* room)
{
	switch (command) {
	case EAST:
		//Move player east
		if (m_mapPosition.x < MAZE_WIDTH - 1)
			m_mapPosition.x++;
		return;
	case WEST:
		//Move player west
		if (m_mapPosition.x > 0)
			m_mapPosition.x--;
		return;
	case NORTH:
		//Move player north
		if (m_mapPosition.y > 0)
			m_mapPosition.y--;
		return;
	case SOUTH:
		//Move player south
		if (m_mapPosition.y < MAZE_HEIGHT - 1)
			m_mapPosition.y++;
		return;
	case LOOK:
		//Tell player what is in the room
		room->lookAt();
		break;
	case FIGHT:
		//Attack the enemy in the room
		attack(room->getEnemy());
		break;
	case PICKUP:
		//Pickup what's in the room
		pickup(room);
		break;
	default:
		//Whoops
		std::cout << EXTRA_OUTPUT_POS << RESET_COLOR;
		std::cout << "You try, but you just can't do it." << std::endl;
		break;
	}

	std::cout << INDENT << "Press 'Enter' to continue.";
	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());
	std::cin.get();
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
	for (std::vector<Powerup*>::iterator it = m_powerups.begin(); it < m_powerups.end(); it++) {
		std::cout << (*it)->getName() << "\t";
	}
}

void Player::drawDescription()
{
}

void Player::lookAt()
{
	std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "Hmm, I look good!" << std::endl;
}

void Player::pickup(Room* room)
{
	//Get powerup and food
	Powerup* powerup = room->getPowerup();
	Food* food = room->getFood();

	//Set output position and color
	std::cout << EXTRA_OUTPUT_POS << RESET_COLOR;

	if (powerup != nullptr) {
		std::cout << "You pick up the " << powerup->getName() << "." << std::endl;
		//Add the powerup to the inventory
		addPowerup(powerup);
		//Remove the powerup from the room
		room->removeGameObject(powerup);
	}
	else if (food != nullptr) {
		//CONSUME
		m_hitPoints += food->getHP();
		std::cout << "You feel refreshed. You have recovered " << food->getHP() <<
			" hit points and now have " << m_hitPoints << "." << std::endl;
		//Remove the food from the room
		room->removeGameObject(food);
	}
	else {
		std::cout << "There is nothing here to pick up." << std::endl;
	}
}

void Player::attack(Enemy* enemy)
{
	//Set output position and color
	std::cout << EXTRA_OUTPUT_POS << RESET_COLOR;

	if (enemy != nullptr) {
		//Damage the enemy
		enemy->onAttacked(m_attackPoints);

		//Check enemy status
		if (!enemy->isAlive()) {
			std::cout << "You fight a grue and kill it." << std::endl;
		}
		else {
			int damage = enemy->getAP() - m_defensePoints;
			if (damage < 1) damage = 1;
			m_hitPoints -= damage;

			std::cout << "You fight a grue and take " << damage << " points of damage. "
				<< "You have " << m_hitPoints << " remaining." << std::endl;
			std::cout << INDENT << "The grue has " << enemy->getHP() << " hit points remaining."
				<< std::endl;
		}
	}
	else {
		std::cout << "There is no one here to fight." << std::endl;
	}
}

/*
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
*/