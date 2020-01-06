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
const char* SAVE_CURSOR_POS = "\x1b[s";
const char* RESTORE_CURSOR_POS = "\x1b[u";

const char* ICON_PLAYER = "@";
const char* ICON_EMPTY = "\xb0";
const char* ICON_ENEMY = "\x94";
const char* ICON_TREASURE = "$";
const char* ICON_FOOD = "\xed";
const char* ICON_ENTRANCE = "\xe9";
const char* ICON_EXIT = "\xfe";

void main()
{
	const int EMPTY		= 0;
	const int ENEMY		= 1;
	const int TREASURE	= 2;
	const int FOOD		= 3;
	const int ENTRANCE	= 4;
	const int EXIT		= 5;
	const int MAX_RANDOM_TYPE = FOOD + 1;

	const int MAZE_WIDTH	= 10;
	const int MAZE_HEIGHT	= 6;

	const int INDENT_X			= 5;
	const int ROOM_DESC_Y		= 8;
	const int MAP_Y				= 13;
	const int PLAYER_INPUT_X	= 30;
	const int PLAYER_INPUT_Y	= 11;

	const int WEST	= 4;
	const int EAST	= 6;
	const int NORTH	= 8;
	const int SOUTH	= 2;

	//Set output mode to handle virtual terminal sequences
	DWORD dwMode = 0;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);

	//Create a 2D array
	int rooms[MAZE_HEIGHT][MAZE_WIDTH];

	//Set the random seed
	srand(time(nullptr));

	//Fill the arrays with random room types
	for (int y = 0; y < MAZE_HEIGHT; y++) {
		for (int x = 0; x < MAZE_WIDTH; x++) {
			int type = rand() % (MAX_RANDOM_TYPE * 2);
			if (type < MAX_RANDOM_TYPE)
				rooms[y][x] = type;
			else
				rooms[y][x] = EMPTY;
		}
	}
	//Set the entrance and exit of the maze
	rooms[0][0] = ENTRANCE;
	rooms[MAZE_HEIGHT - 1][MAZE_WIDTH - 1] = EXIT;

	//Greet the player
	cout << TITLE << MAGENTA << "Welcome to ZORP!" << RESET_COLOR << endl;
	cout << INDENT << "ZORP is a game of a game of adventure, danger, and low cunning." << endl;
	cout << INDENT << "It is definitely not related to any other text-based adventure game." << endl << endl;
	
	//Restore cursor position
	cout << SAVE_CURSOR_POS;

	cout << CSI << MAP_Y << ";" << 0 << "H";
	for (int y = 0; y < MAZE_HEIGHT; y++) {
		cout << INDENT;
		for (int x = 0; x < MAZE_WIDTH; x++) {
			//cout << "[ " << rooms[y][x] << " ] ";
			cout << "[ ";
			switch (rooms[y][x]) {
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
		cout << endl;
	}

	bool gameOver = false;
	int playerX = 0;
	int playerY = 0;

	//Game Loop
	while (!gameOver) {
		//Prepare screen for output
		//Move cursor to start of the 1st Q, then up 1, delete and insert 4 lines
		cout << RESTORE_CURSOR_POS << CSI << "A" << CSI << "4M" << CSI << "4L" << endl;

		//Write description of current room
		switch (rooms[playerY][playerX]) {
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
			gameOver = true;
			continue;
		}

		//List the directions the player can take
		cout << INDENT << "You can see paths leading to the " <<
			((playerX > 0) ? "west, " : "") <<
			((playerX < MAZE_WIDTH - 1) ? "east, " : "") <<
			((playerY > 0) ? "north, " : "") <<
			((playerY < MAZE_HEIGHT - 1) ? "south, " : "") << endl;

		cout << INDENT << "Where to now?";

		int x = INDENT_X + (6 * playerX) + 3;
		int y = MAP_Y + playerY;

		//Draw the player's position on the map
		//Move cursor to map pos and delete character at current position
		cout << CSI << y << ";" << x << "H";
		cout << MAGENTA << ICON_PLAYER;

		//Move cursor to position for player to enter input
		cout << CSI << PLAYER_INPUT_Y << ";" << PLAYER_INPUT_X << "H" << YELLOW;

		//Clear the input buffer, ready for player input
		cin.clear();
		cin.ignore(cin.rdbuf()->in_avail());

		int direction = 0;
		cin >> direction;
		cout << RESET_COLOR;

		if (cin.fail())
			continue;	//Restart the loop

		//Before updating the player position, redraw the old room icon
		cout << CSI << y << ";" << x << "H";
		switch (rooms[playerY][playerX]) {
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
		cout << RESET_COLOR;

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

	cout << endl << INDENT << "Press 'Enter' to exit the program." << endl;

	cin.clear();
	cin.ignore(cin.rdbuf()->in_avail());
	cin.get();

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
