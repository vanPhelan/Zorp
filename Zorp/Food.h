#pragma once

class Food {
public:
	Food() : m_hitPoints{ 10 } {}
	~Food() {}

	int getHP() { return m_hitPoints; }

private:
	int m_hitPoints;
};
