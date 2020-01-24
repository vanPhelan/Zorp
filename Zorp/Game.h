#pragma once
#include "Room.h"
#include "Player.h"
#include "GameDefines.h"

class Enemy;
class Food;
class Powerup;

class Game
{
public:
	Game();
	~Game();

	bool startup();
	void update();
	void draw();

	bool isGameOver();

	Powerup* findPowerup(const char* name, bool isLoading) const;

private:
	bool enableVirtualTerminal();
	void initializeMap();
	void initializeEnemies();
	void initializeFood();
	void initializePowerups();

	void drawWelcomeMessage();
	void drawMap();
	void drawValidDirections();

	int getCommand();
	void save();
	bool load();

private:
	bool m_gameOver;

	Room m_map[MAZE_HEIGHT][MAZE_WIDTH];

	int m_enemyCount;
	Enemy* m_enemies;

	int m_foodCount;
	Food* m_food;

	int m_powerupCount;
	Powerup* m_powerups;

	int m_tempPowerupCount;
	Powerup* m_tempPowerups;

	Player m_player;
};

