#include "pch.h"
#include "Food.h"
#include "GameDefines.h"
#include <iostream>

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
