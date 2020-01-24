#pragma once
#include "Character.h"
#include <vector>

class Room;
class Enemy;

class Player : public Character
{
public:
	Player();
	Player(int x, int y);
	~Player() {}

	void executeCommand(int command, Room* room);

	void draw();
	void drawDescription();
	void lookAt();

private:
	void pickup(Room* room);
	void attack(Enemy* enemy);
};

