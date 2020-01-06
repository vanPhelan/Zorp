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
const char* YELLOW = "\x1b[93m";
const char* MAGENTA = "\x1b[95m";
const char* RESET_COLOR = "\x1b[0m";
const char* SAVE_CURSOR_POS = "\x1b[s";
const char* RESTORE_CURSOR_POS = "\x1b[u";

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

	//Set output mode to handle virtual terminal sequences
	DWORD dwMode = 0;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);

    //std::cout << "Hello World!\n";

	int height = 0;
	char firstInitial = 0;
	int hitPoints = 0;

	//Create a 2D array
	int rooms[MAZE_HEIGHT][MAZE_WIDTH];

	//Set the random seed
	srand(time(nullptr));

	//Fill the arrays with random room types
	for (int y = 0; y < MAZE_HEIGHT; y++) {
		for (int x = 0; x < MAZE_WIDTH; x++) {
			rooms[y][x] = rand() % MAX_RANDOM_TYPE;
		}
	}
	//Set the entrance and exit of the maze
	rooms[0][0] = ENTRANCE;
	rooms[MAZE_HEIGHT - 1][MAZE_WIDTH - 1] = EXIT;

	//Greet the player
	cout << TITLE << MAGENTA << "Welcome to ZORP!" << RESET_COLOR << endl;
	cout << INDENT << "ZORP is a game of a game of adventure, danger, and low cunning." << endl;
	cout << INDENT << "It is definitely not related to any other text-based adventure game." << endl << endl;
	cout << INDENT << "First, some questions..." << endl;

	//Restore cursor position
	cout << SAVE_CURSOR_POS;

	cout << endl << endl << endl << endl;
	for (int y = 0; y < MAZE_HEIGHT; y++) {
		cout << INDENT;
		for (int x = 0; x < MAZE_WIDTH; x++) {
			cout << "[ " << rooms[y][x] << " ] ";
		}
		cout << endl;
	}

	//Restore cursor position
	cout << RESTORE_CURSOR_POS;

	//Get player's height
	cout << INDENT << "How tall are you?" << INDENT << YELLOW;
	cin >> height;
	cout << RESET_COLOR << endl;

	if (cin.fail()) {
		cout << INDENT << "You have failed the first challenge and are eaten by a grue." << endl;
	}
	else {
		cout << INDENT << "You entered " << height << "." << endl;
	}

	cin.clear();
	cin.ignore(cin.rdbuf()->in_avail());
	cin.get();

	//Move the cursor to the start of the first question
	cout << RESTORE_CURSOR_POS;
	//Delete the next 3 lines of text
	cout << CSI << "3M";
	//Insert 3 lines
	cout << CSI << "3L";

	//Get player's first initial
	cout << INDENT << "What is the first letter of your name?" << INDENT << YELLOW;

	cin.clear();
	cin.ignore(cin.rdbuf()->in_avail());
	cin >> firstInitial;
	cout << RESET_COLOR << endl;

	if (cin.fail() || !isalpha(firstInitial)) {
		cout << INDENT << "You have failed the second challenge and are eaten by a grue." << endl;
	}
	else {
		cout << INDENT << "You entered " << firstInitial << "." << endl;
	}

	cin.clear();
	cin.ignore(cin.rdbuf()->in_avail());
	cin.get();

	//Move the cursor to the start of the first question
	cout << RESTORE_CURSOR_POS;
	cout << CSI << "A";		//cursor up 1
	cout << CSI << "4M";	//delete next 4 lines
	cout << CSI << "4L";	//insert 4 lines

	//Calculate player's hit points
	if (firstInitial != 0) {
		hitPoints = (float)height / (firstInitial * 0.02f);
	}
	else {
		hitPoints = 0;
	}
	cout << INDENT << "Using a complex deterministic algorithm, it has been calculated that you have "
		<< hitPoints << " hit points." << endl;

	//system("pause");
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
