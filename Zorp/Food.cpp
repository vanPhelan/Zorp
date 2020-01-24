#include "pch.h"
#include "Food.h"
#include "GameDefines.h"
#include <iostream>
#include <string>

Food::Food() : m_hitPoints{ 10 }
{
	m_priority = PRIORITY_FOOD;
}

Food::Food(Point2D position) : GameObject{ position }, m_hitPoints{ 10 }
{
	m_priority = PRIORITY_FOOD;
}

void Food::draw()
{
	std::cout << WHITE << ICON_FOOD << RESET_COLOR;
}

void Food::drawDescription()
{
	std::cout << INDENT << "You smell a recently extinguished campfire, " << 
		"perhaps left by a previous traveller." << std::endl;
}

void Food::lookAt()
{
	std::cout << EXTRA_OUTPUT_POS << RESET_COLOR <<
		"There is some food here. It should be edible." << std::endl;
}

void Food::save(std::ofstream& out)
{
	if (!out.is_open())
		return;

	out << m_priority << ",";
	out << m_mapPosition.x << ",";
	out << m_mapPosition.y << ",";
	out << m_hitPoints << std::endl;
}

bool Food::load(std::ifstream& in, const Game * game)
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
	in.getline(buffer, 50);
	if (in.rdstate() || buffer[0] == 0)
		return false;
	m_hitPoints = std::stoi(buffer);

	return true;
}
