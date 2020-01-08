#pragma once
#include "Room.h"
#include "Player.h"
#include "GameDefines.h"

class Game
{
public:
	Game();
	~Game();

	bool startup();
	void update();
	void draw();

	bool isGameOver();

private:
	bool enableVirtualTerminal();
	void initializeMap();

	void drawWelcomeMessage();
	void drawMap();
	void drawValidDirections();

	int getCommand();

private:
	bool m_gameOver;
	Room m_map[MAZE_HEIGHT][MAZE_WIDTH];
	Player m_player;
};

