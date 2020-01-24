#include "pch.h"
#include "Room.h"
#include "GameObject.h"
#include "Enemy.h"
#include "Food.h"
#include "Powerup.h"
#include "GameDefines.h"
#include <iostream>
#include <algorithm>

Room::Room() :
	m_type{ EMPTY },
	m_mapPosition{ 0, 0 }
{
}

Enemy* Room::getEnemy()
{
	for (GameObject* obj : m_objects) {
		Enemy* enemy = dynamic_cast<Enemy*>(obj);
		if (enemy != nullptr) {
			return enemy;
		}
	}
	return nullptr;
}

Powerup* Room::getPowerup()
{
	for (GameObject* obj : m_objects) {
		Powerup* powerup = dynamic_cast<Powerup*>(obj);
		if (powerup != nullptr) {
			return powerup;
		}
	}
	return nullptr;
}

Food* Room::getFood()
{
	for (GameObject* obj : m_objects) {
		Food* food = dynamic_cast<Food*>(obj);
		if (food != nullptr) {
			return food;
		}
	}
	return nullptr;
}

void Room::addGameObject(GameObject* object)
{
	//Add the GameObject
	m_objects.push_back(object);
	//Sort the GameObjects
	std::sort(m_objects.begin(), m_objects.end(), GameObject::compare);
}

void Room::removeGameObject(GameObject* object)
{
	for (auto it = m_objects.begin(); it != m_objects.end(); it++) {
		//If the object is found in the list...
		if (*it == object) {
			//...remove it
			(*it)->setPosition({ -1,-1 });
			m_objects.erase(it);
			return;
		}
	}
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
		if (m_objects.size() > 0) {
			m_objects[0]->draw();
		}
		else {
			std::cout << GREEN << ICON_EMPTY<< RESET_COLOR;
		}
		break;
	case ENTRANCE:
		std::cout << WHITE << ICON_ENTRANCE << RESET_COLOR;
		break;
	case EXIT:
		std::cout << WHITE << ICON_EXIT << RESET_COLOR;
		break;
	}
	std::cout  << " ] ";
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
		if (m_objects.size() > 0) {
			m_objects[0]->drawDescription();
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

void Room::lookAt()
{
	if (m_objects.size() > 0) {
		m_objects[0]->lookAt();
	}
	else {
		std::cout << EXTRA_OUTPUT_POS << RESET_COLOR
			<< "You look around but see nothing worth mentioning."
			<< std::endl;
	}
}
