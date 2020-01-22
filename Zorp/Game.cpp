#include "pch.h"
#include "Game.h"
#include "Enemy.h"
#include "Food.h"
#include "Powerup.h"
#include <iostream>
#include <Windows.h>
#include <random>
#include <time.h>

Game::Game() : m_gameOver{ false }
{
}


Game::~Game()
{
	delete[] m_powerups;
	delete[] m_enemies;
	delete[] m_food;
}

bool Game::startup()
{
	if (!enableVirtualTerminal()) {

	}
	//Set the random seed
	srand(time(nullptr));
	//Initialize the map and its contents
	initializeMap();
	initializeEnemies();
	initializeFood();
	initializePowerups();
	//Welcome the player
	drawWelcomeMessage();

	return true;
}

void Game::update()
{
	//Check if the player is at the exit
	Point2D playerPos = m_player.getPosition();
	if (m_map[playerPos.y][playerPos.x].getType() == EXIT) {
		m_gameOver = true;
		return;
	}
	//Get command from the player
	int command = getCommand();

	//Exit game if needed
	if (command == QUIT) {
		m_gameOver = true;
		return;
	}

	//Execute the command
	m_player.executeCommand(command, &m_map[playerPos.y][playerPos.x]);

	//Kill dead enemies
	for (int i = 0; i < m_enemyCount; i++) {
		if (!m_enemies[i].isAlive()) {
			Point2D pos = m_enemies[i].getPosition();
			m_map[pos.y][pos.x].removeGameObject(&m_enemies[i]);
		}
	}
}

void Game::draw()
{
	//Draw the description of the current room
	Point2D playerPos = m_player.getPosition();
	m_map[playerPos.y][playerPos.x].drawDescription();
	//List the directions the player can take
	drawValidDirections();
	//Redraw the map
	drawMap();
	//Draw the player on the map
	m_player.draw();
}

bool Game::isGameOver()
{
	return m_gameOver;
}

bool Game::enableVirtualTerminal()
{
	//Set output mode to handle virtual terminal sequences
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE) {
		return false;
	}
	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode)) {
		return false;
	}
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (SetConsoleMode(hOut, dwMode)) {
		return false;
	}
	return true;
}

void Game::initializeMap()
{
	//Set room positions
	for (int y = 0; y < MAZE_HEIGHT; y++) {
		for (int x = 0; x < MAZE_WIDTH; x++) {
			m_map[y][x].setPosition(Point2D{ x, y });
		}
	}
	//Set the entrance and exit of the maze
	m_map[0][0].setType(ENTRANCE);
	m_map[MAZE_HEIGHT - 1][MAZE_WIDTH - 1].setType(EXIT);
}

void Game::initializeEnemies()
{
	//Create a dynamic array of enemies
	m_enemyCount = 1 + rand() % 4;
	m_enemies = new Enemy[m_enemyCount];

	//Place each enemy in a random room on the map
	for (int i = 0; i < m_enemyCount; i++) {
		//Ensure enemies are not placed on or adjecent to entrance or exit
		int x = 2 + (rand() % (MAZE_WIDTH - 3));
		int y = 2 + (rand() % (MAZE_HEIGHT - 3));

		//Set the enemy's position
		m_enemies[i].setPosition({ x, y });
		//Set the room's enemy
		m_map[y][x].addGameObject(&m_enemies[i]);
	}
}

void Game::initializeFood()
{
	//Create a dynamic array of food
	m_foodCount = 3;
	m_food = new Food[m_foodCount];

	//Place each food in a random room on the map
	for (int i = 0; i < m_foodCount; i++) {
		int x = rand() % (MAZE_WIDTH - 1);
		int y = rand() % (MAZE_HEIGHT - 1);

		//Set the room's food
		m_map[y][x].addGameObject(&m_food[i]);
	}
}

void Game::initializePowerups()
{
	//Create a dynamic array of powerups
	m_powerupCount = 3;
	m_powerups = new Powerup[m_powerupCount];

	//Place each powerup in a random room on the map
	for (int i = 0; i < m_powerupCount; i++) {
		char name[30] = "";

		int x = rand() % (MAZE_WIDTH - 1);
		int y = rand() % (MAZE_HEIGHT - 1);

		//Add the first part of the item name
		switch (i) {
		case 0:
			strcpy_s(name, "potion of ");
			m_powerups[i].setHealthMultiplier(1.1f);
			break;
		case 1:
			strcpy_s(name, "sword of ");
			m_powerups[i].setAttackMultiplier(1.1f);
			break;
		case 2:
			strcpy_s(name, "shield of ");
			m_powerups[i].setDefenseMultiplier(1.1f);
			break;
		}

		//Add the second part of the item name
		strncat_s(name, descriptors[rand() % 15], 30);
		//Set the power's name
		m_powerups[i].setName(name);
		//Set the room's powerup
		m_map[y][x].addGameObject(&m_powerups[i]);
	}
}

void Game::drawWelcomeMessage()
{
	//Greet the player
	std::cout << TITLE << MAGENTA << "Welcome to ZORP!" << RESET_COLOR << std::endl;
	std::cout << INDENT << "ZORP is a game of a game of adventure, danger, and low cunning." << std::endl;
	std::cout << INDENT << "It is definitely not related to any other text-based adventure game." << std::endl << std::endl;
}

void Game::drawMap()
{
	//Reset draw colors
	std::cout << RESET_COLOR;
	for (int y = 0; y < MAZE_HEIGHT; y++) {
		std::cout << INDENT;
		for (int x = 0; x < MAZE_WIDTH; x++) {
			m_map[y][x].draw();
		}
		std::cout << std::endl;
	}
}

void Game::drawValidDirections()
{
	Point2D position = m_player.getPosition();

	//Reset draw color
	std::cout << RESET_COLOR;
	//Reposition cursor
	std::cout << CSI << MOVEMENT_DESC_Y + 1 << ";" << 0 << "H";
	//List the directions the player can take
	std::cout << INDENT << "You can see paths leading to the " <<
		((position.x > 0) ? "west, " : "") <<
		((position.x < MAZE_WIDTH - 1) ? "east, " : "") <<
		((position.y > 0) ? "north, " : "") <<
		((position.y < MAZE_HEIGHT - 1) ? "south, " : "") << std::endl;
}

int Game::getCommand()
{
	//Create the input buffer
	char input[50] = "\0";

	//Reset draw color
	std::cout << RESET_COLOR;
	//Reposition cursor
	std::cout << CSI << PLAYER_INPUT_Y << ";" << 0 << "H";
	//Clear existing text
	std::cout << CSI << "5M";
	//Insert 5 blank lines to ensure the inventory output remains correct
	std::cout << CSI << "5L";

	std::cout << INDENT << "Enter a command.";

	//Move cursor to position for player to enter input
	std::cout << CSI << PLAYER_INPUT_Y << ";" << PLAYER_INPUT_X << "H" << YELLOW;

	//Clear the input buffer, ready for player input
	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());

	std::cin >> input;
	std::cout << RESET_COLOR;

	bool firstWordIsMove = false;
	bool firstWordIsPick = false;
	while (input) {
		if (strcmp(input, "move") == 0) {
			firstWordIsMove = true;
		}
		else if (firstWordIsMove) {
			if (strcmp(input, "north") == 0)
				return NORTH;
			if (strcmp(input, "south") == 0)
				return SOUTH;
			if (strcmp(input, "east") == 0)
				return EAST;
			if (strcmp(input, "west") == 0)
				return WEST;
		}

		if (strcmp(input, "look") == 0) {
			return LOOK;
		}

		if (strcmp(input, "fight") == 0) {
			return FIGHT;
		}

		if (strcmp(input, "pick") == 0) {
			firstWordIsPick = true;
		}
		else if (firstWordIsPick) {
			if (strcmp(input, "up") == 0)
				return PICKUP;
		}

		if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) {
			return QUIT;
		}

		char next = std::cin.peek();
		if (next == '\n' || next == EOF)
			break;
		std::cin >> input;
	}

	return 0;
}
