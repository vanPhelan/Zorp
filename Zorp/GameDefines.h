#pragma once

const char* const CLEAR = "\x1b[2J\x1b[H";
const char* const CSI = "\x1b[";
const char* const TITLE = "\x1b[5;20H";
const char* const INDENT = "\x1b[5C";
const char* const RED = "\x1b[91m";
const char* const GREEN = "\x1b[92m";
const char* const YELLOW = "\x1b[93m";
const char* const BLUE = "\x1b[94m";
const char* const MAGENTA = "\x1b[95m";
const char* const CYAN = "\x1b[96m";
const char* const WHITE = "\x1b[97m";
const char* const RESET_COLOR = "\x1b[0m";

const char* const EXTRA_OUTPUT_POS = "\x1b[21;6H";
const char* const INVENTORY_OUTPUT_POS = "\x1b[24;6H";

const int EMPTY		= 0;
const int ENTRANCE	= EMPTY + 1;
const int EXIT		= ENTRANCE + 1;

const int PRIORITY_DEFAULT = 5;
const int PRIORITY_PLAYER = 0;
const int PRIORITY_ENEMY = 1;
const int PRIORITY_POWERUP = 2;
const int PRIORITY_FOOD = 3;

const char* const ICON_PLAYER = "@";
const char* const ICON_EMPTY = "\xb0";
const char* const ICON_ENEMY = "\x94";
const char* const ICON_TREASURE = "$";
const char* const ICON_FOOD = "\xed";
const char* const ICON_ENTRANCE = "\xe9";
const char* const ICON_EXIT = "\xfe";

const int MAZE_WIDTH = 10;
const int MAZE_HEIGHT = 6;

const int INDENT_X = 5;
const int ROOM_DESC_Y = 8;
const int MOVEMENT_DESC_Y = 9;
const int MAP_Y = 12;

const int PLAYER_INPUT_X = 30;
const int PLAYER_INPUT_Y = 19;

const int WEST = 4;
const int EAST = 6;
const int NORTH = 8;
const int SOUTH = 2;

const int LOOK = 9;
const int FIGHT = 10;
const int PICKUP = 11;
const int QUIT = 12;
const int SAVE = 13;
const int LOAD = 14;

static const char descriptors[15][30] = {
		"indifference", "invisibility", "invulnerability", "incontinence",
		"improbability", "impatience", "indecision", "inspiration",
		"independence", "incurability", "integration", "invocation",
		"inferno", "indigestion", "inoculation"
};
