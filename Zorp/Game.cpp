#include "pch.h"
#include "Game.h"
#include "Enemy.h"
#include "Food.h"
#include "Powerup.h"
#include <iostream>
#include <Windows.h>
#include <random>
#include <time.h>
#include <fstream>
#include <string>

Game::Game() : m_gameOver{ false }, m_tempPowerups{ nullptr }
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

Powerup* Game::findPowerup(const char * name, bool isLoading) const
{
	if (!isLoading) {
		//Search the list of powerups
		for (int i = 0; i < m_powerupCount; i++) {
			if (strcmp(name, m_powerups[i].getName()) == 0)
				return &m_powerups[i];
		}
	}
	else {
		//Search the list of loading powerups
		for (int i = 0; i < m_tempPowerupCount; i++) {
			if (strcmp(name, m_tempPowerups[i].getName()) == 0)
				return &m_tempPowerups[i];
		}
	}
	//Nothing was found
	return nullptr;
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
		//Add the enemy to the room
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

		//Set the food's position
		m_food[i].setPosition({ x, y });
		//Add the food to the room
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
		//Set the powerup's name
		m_powerups[i].setName(name);
		//Set the powerup's position
		m_powerups[i].setPosition({ x, y });
		//Add the powerup to the room
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

		if (strcmp(input, "save") == 0) {
			save();
			return SAVE;
		}

		if (strcmp(input, "load") == 0) {
			if (!load()) {
				std::cout << EXTRA_OUTPUT_POS << RED <<
					"Could not load zorpsave.dat." << std::endl;
			}
			return LOAD;
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

void Game::save()
{
	std::ofstream out;

	//Open the file for output in binary mode, and truncate
	out.open("zorpsave.dat",
		std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);

	if (out.is_open()) {
		//You can't save when dead
		if (m_gameOver) {
			std::cout << EXTRA_OUTPUT_POS <<
				"You cannot save while dead." << std::endl;
		}
		else {
			//Save the powerups
			out.write((char*)&m_powerupCount, sizeof(int));
			for (int i = 0; i < m_powerupCount; i++) {
				m_powerups[i].save(out);
			}
			//Save the enemies
			out.write((char*)&m_enemyCount, sizeof(int));
			for (int i = 0; i < m_enemyCount; i++) {
				m_enemies[i].save(out);
			}
			//Save the food
			out.write((char*)&m_foodCount, sizeof(int));
			for (int i = 0; i < m_foodCount; i++) {
				m_food[i].save(out);
			}
			//Save the player
			m_player.save(out);
		}
	}
	else {
		//Could not open the file
		std::cout << EXTRA_OUTPUT_POS << RED <<
			"Could not open zorpsave.dat." << RESET_COLOR << std::endl;
	}

	out.flush();
	out.close();
}

bool Game::load()
{
	std::ifstream in;

	//Open the file for input
	in.open("zorpsave.dat", std::ifstream::in | std::ifstream::binary);

	if (!in.is_open()) {
		return false;
	}

	//Clear the temporary list
	if (m_tempPowerups != nullptr)
		delete[] m_tempPowerups;

	//Load the powerups
	in.read((char*)&m_tempPowerupCount, sizeof(int));
	if (in.rdstate() || m_tempPowerupCount < 0)
		return false;
	//Read the powerups
	m_tempPowerups = new Powerup[m_tempPowerupCount];
	for (int i = 0; i < m_tempPowerupCount; i++) {
		if (m_tempPowerups[i].load(in, this) == false) {
			delete[] m_tempPowerups;
			m_tempPowerups = nullptr;
			return false;
		}
	}

	//Load the enemies
	int enemyCount;
	in.read((char*)&enemyCount, sizeof(int));
	if (in.rdstate() || enemyCount < 0)
		return false;
	//Read the enemies
	Enemy* enemies = new Enemy[enemyCount];
	for (int i = 0; i < enemyCount; i++) {
		if (enemies[i].load(in, this) == false) {
			delete[] enemies;
			delete[] m_tempPowerups;
			m_tempPowerups = nullptr;
			return false;
		}
	}

	//Load the food
	int foodCount;
	in.read((char*)&foodCount, sizeof(int));
	if (in.rdstate() || foodCount < 0)
		return false;
	//Read the food
	Food* food = new Food[foodCount];
	for (int i = 0; i < foodCount; i++) {
		if (food[i].load(in, this) == false) {
			delete[] food;
			delete[] enemies;
			delete[] m_tempPowerups;
			m_tempPowerups = nullptr;
			return false;
		}
	}

	//Load the player
	Player player;
	if (player.load(in, this) == false) {
		delete[] food;
		delete[] enemies;
		delete[] m_tempPowerups;
		m_tempPowerups = nullptr;
		return false;
	}

	//Rebuild the level

	//Clear the rooms
	for (int y = 0; y < MAZE_HEIGHT; y++) {
		for (int x = 0; x < MAZE_WIDTH; x++) {
			m_map[y][x].clearGameObjects();
		}
	}

	//Add the powerups
	delete[] m_powerups;
	m_powerups = m_tempPowerups;
	m_powerupCount = m_tempPowerupCount;
	m_tempPowerups = nullptr;
	//Place the powerups
	for (int i = 0; i < m_powerupCount; i++) {
		Point2D pos = m_powerups[i].getPosition();
		if (pos.x >= 0 && pos.x < MAZE_WIDTH && pos.y >= 0 && pos.y < MAZE_HEIGHT) {
			m_map[pos.y][pos.x].addGameObject(&m_powerups[i]);
		}
	}

	//Add the enemies
	delete[] m_enemies;
	m_enemies = enemies;
	m_enemyCount = enemyCount;
	//Place the enemies
	for (int i = 0; i < m_enemyCount; i++) {
		Point2D pos = m_enemies[i].getPosition();
		if (m_enemies[i].isAlive() && pos.x >= 0 && pos.x < MAZE_WIDTH && pos.y >= 0 && pos.y < MAZE_HEIGHT) {
			m_map[pos.y][pos.x].addGameObject(&m_enemies[i]);
		}
	}

	//Add the food
	delete[] m_food;
	m_food = food;
	m_foodCount = foodCount;
	//Place the food
	for (int i = 0; i < m_foodCount; i++) {
		Point2D pos = m_food[i].getPosition();
		if (pos.x >= 0 && pos.x < MAZE_WIDTH && pos.y >= 0 && pos.y < MAZE_HEIGHT) {
			m_map[pos.y][pos.x].addGameObject(&m_food[i]);
		}
	}

	//Add the player
	m_player = player;

	return true;
}
