// Zorp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

using namespace std;

void main()
{
    //std::cout << "Hello World!\n";

	int height = 0;
	char firstInitial = 0;
	int hitPoints = 0;

	//Greet the player
	cout << "Welcome to ZORP!" << endl
		<< "ZORP is a game of a game of adventure, danger, and low cunning." << endl;

	//Get player's height
	cout << "How tall are you?" << endl;
	cin >> height;
	if (cin.fail()) {
		cout << "You have failed the first challenge and are eaten by a grue." << endl;
	}
	else {
		cout << "You entered " << height << "." << endl;
	}
	cin.clear();
	cin.ignore(cin.rdbuf()->in_avail());

	//Get player's first initial
	cout << "What is the first letter of your name?" << endl;
	cin >> firstInitial;
	if (cin.fail() || !isalpha(firstInitial)) {
		cout << "You have failed the second challenge and are eaten by a grue." << endl;
	}
	else {
		cout << "You entered " << firstInitial << "." << endl;
	}
	cin.clear();
	cin.ignore(cin.rdbuf()->in_avail());

	//Calculate player's hit points
	if (firstInitial != 0) {
		hitPoints = (float)height / (firstInitial * 0.02f);
	}
	else {
		hitPoints = 0;
	}
	cout << "\nUsing a complex deterministic algorithm, it has been calculated that you have "
		<< hitPoints << " hit points." << endl;

	//system("pause");
	cout << endl << "Press 'Enter' to exit the program." << endl;
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
