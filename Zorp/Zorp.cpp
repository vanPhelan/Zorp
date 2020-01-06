// Zorp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <random>
#include <time.h>

using namespace std;

const char* CLEAR = "\x1b[2J\x1b[H";
const char* CSI = "\x1b[";
const char* TITLE = "\x1b[5;20H";
const char* INDENT = "\x1b[5C";
const char* RED = "\x1b[91m";
const char* GREEN = "\x1b[92m";
const char* YELLOW = "\x1b[93m";
const char* BLUE = "\x1b[94m";
const char* MAGENTA = "\x1b[95m";
const char* CYAN = "\x1b[96m";
const char* WHITE = "\x1b[97m";
const char* RESET_COLOR = "\x1b[0m";
//const char* SAVE_CURSOR_POS = "\x1b[s";
//const char* RESTORE_CURSOR_POS = "\x1b[u";

const int EMPTY		= 0;
const int ENEMY		= 1;
const int TREASURE	= 2;
const int FOOD		= 3;
const int ENTRANCE	= 4;
const int EXIT		= 5;

const int MAX_RANDOM_TYPE = FOOD + 1;

const char* ICON_PLAYER = "@";
const char* ICON_EMPTY = "\xb0";
const char* ICON_ENEMY = "\x94";
const char* ICON_TREASURE = "$";
const char* ICON_FOOD = "\xed";
const char* ICON_ENTRANCE = "\xe9";
const char* ICON_EXIT = "\xfe";

const int MAZE_WIDTH = 10;
const int MAZE_HEIGHT = 6;

const int INDENT_X = 5;
const int ROOM_DESC_Y = 8;
const int MOVEMENT_DESC_Y = 9;
const int MAP_Y = 13;
const int PLAYER_INPUT_X = 30;
const int PLAYER_INPUT_Y = 11;

const int WEST = 4;
const int EAST = 6;
const int NORTH = 8;
const int SOUTH = 2;

bool enableVirtualTerminal() {
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

void initialize(int map[MAZE_HEIGHT][MAZE_WIDTH]) {
	srand(time(nullptr));

	//Fill the arrays with random room types
	for (int y = 0; y < MAZE_HEIGHT; y++) {
		for (int x = 0; x < MAZE_WIDTH; x++) {
			int type = rand() % (MAX_RANDOM_TYPE * 2);
			if (type < MAX_RANDOM_TYPE)
				map[y][x] = type;
			else
				map[y][x] = EMPTY;
		}
	}
	//Set the entrance and exit of the maze
	map[0][0] = ENTRANCE;
	map[MAZE_HEIGHT - 1][MAZE_WIDTH - 1] = EXIT;
}

void drawWelcomeMessage() {
	//Greet the player
	cout << TITLE << MAGENTA << "Welcome to ZORP!" << RESET_COLOR << endl;
	cout << INDENT << "ZORP is a game of a game of adventure, danger, and low cunning." << endl;
	cout << INDENT << "It is definitely not related to any other text-based adventure game." << endl << endl;
}

void drawRoom(int map[MAZE_HEIGHT][MAZE_WIDTH], int x, int y) {
	//Find the console output position
	int outX = INDENT_X + (6 * x) + 1;
	int outY = MAP_Y + y;

	//Jump to the correct location
	cout << CSI << outY << ";" << outX << "H";
	//Draw the room
	cout << "[ ";
	switch (map[y][x]) {
	case EMPTY:
		cout << GREEN << ICON_EMPTY;
		break;
	case ENEMY:
		cout << RED << ICON_ENEMY;
		break;
	case TREASURE:
		cout << YELLOW << ICON_TREASURE;
		break;
	case FOOD:
		cout << CYAN << ICON_FOOD;
		break;
	case ENTRANCE:
		cout << WHITE << ICON_ENTRANCE;
		break;
	case EXIT:
		cout << WHITE << ICON_EXIT;
		break;
	}
	cout << RESET_COLOR << " ] ";
}

void drawMap(int map[MAZE_HEIGHT][MAZE_WIDTH]) {
	//Reset draw colors
	cout << RESET_COLOR;
	for (int y = 0; y < MAZE_HEIGHT; y++) {
		cout << INDENT;
		for (int x = 0; x < MAZE_WIDTH; x++) {
			drawRoom(map, x, y);
		}
		cout << endl;
	}
}

void drawRoomDescription(int roomType) {
	//Reset the draw color
	cout << RESET_COLOR;
	//Reposition cursor
	cout << CSI << ROOM_DESC_Y << ";" << 0 << "H";
	//Delete and insert 4 lines
	cout << CSI << "4M" << CSI << "4L" << endl;

	//Write description of current room
	switch (roomType) {
	case EMPTY:
		cout << INDENT << "You are in an empty meadow. There is nothing of note here."
			<< endl;
		break;
	case ENEMY:
		cout << INDENT << "BEWARE. An enemy is approaching."
			<< endl;
		break;
	case TREASURE:
		cout << INDENT << "Your journey has been rewarded! You have found some treasure."
			<< endl;
		break;
	case FOOD:
		cout << INDENT << "At last! You collect some food to sustain you on your journey."
			<< endl;
		break;
	case ENTRANCE:
		cout << INDENT << "The entrance you used to enter this maze is blocked. There is no going back."
			<< endl;
		break;
	case EXIT:
		cout << INDENT << "Despite all odds, you made it to the exit. Congratulations."
			<< endl;
	}
}

void drawPlayer(int x, int y) {
	x = INDENT_X + (6 * x) + 3;
	y = MAP_Y + y;

	//Draw the player's position on the map
	//Move cursor to map pos and delete character at current position
	cout << CSI << y << ";" << x << "H";
	cout << MAGENTA << ICON_PLAYER;
}

void drawValidDirections(int x, int y) {
	//Reset draw color
	cout << RESET_COLOR;
	//Reposition cursor
	cout << CSI << MOVEMENT_DESC_Y + 1 << ";" << 0 << "H";
	//List the directions the player can take
	cout << INDENT << "You can see paths leading to the " <<
		((x > 0) ? "west, " : "") <<
		((x < MAZE_WIDTH - 1) ? "east, " : "") <<
		((y > 0) ? "north, " : "") <<
		((y < MAZE_HEIGHT - 1) ? "south, " : "") << endl;
}

int getMovementDirection() {
	//Reset draw color
	cout << RESET_COLOR;
	//Reposition cursor
	cout << CSI << PLAYER_INPUT_Y << ";" << 0 << "H" << YELLOW;
	cout << INDENT << "Where to now?";

	//Move cursor to position for player to enter input
	cout << CSI << PLAYER_INPUT_Y << ";" << PLAYER_INPUT_X << "H" << YELLOW;

	//Clear the input buffer, ready for player input
	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());

	int direction = 0;
	std::cin >> direction;
	cout << RESET_COLOR;

	if (std::cin.fail())
		return 0;
	return direction;
}

void main()
{
	//Create a 2D array
	int rooms[MAZE_HEIGHT][MAZE_WIDTH];

	bool gameOver = false;
	int playerX = 0;
	int playerY = 0;

	enableVirtualTerminal();

	initialize(rooms);

	drawWelcomeMessage();

	drawMap(rooms);

	//Game Loop
	while (!gameOver) {
		drawRoomDescription(rooms[playerY][playerX]);

		drawPlayer(playerX, playerY);

		if (rooms[playerY][playerX] == EXIT) {
			gameOver = true;
			continue;
		}

		drawValidDirections(playerX, playerY);

		int direction = getMovementDirection();

		drawRoom(rooms, playerX, playerY);

		//Move player in input direction
		switch (direction) {
		case EAST:
			if (playerX < MAZE_WIDTH - 1)
				playerX++;
			break;
		case WEST:
			if (playerX > 0)
				playerX--;
			break;
		case NORTH:
			if (playerY > 0)
				playerY--;
			break;
		case SOUTH:
			if (playerY < MAZE_HEIGHT - 1)
				playerY++;
		default:
			break; //Do nothing, restart the loop
		}
	} //End of Game Loop

	cout << RESET_COLOR;

	cout << endl << INDENT << "Press 'Enter' to exit the program." << endl;

	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());
	std::cin.get();

	return;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
