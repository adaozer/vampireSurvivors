#pragma once
#include <fstream>
#include <sstream>
#include "GamesEngineeringBase.h"

class Camera;

struct Position {
	// A struct that I only used once to store a position value
	int x, y;
};

bool circlesIntersect(int x1, int y1, int r1, int x2, int y2, int r2);

bool onScreen(float x, float y, float w, float h, Camera& cam, const GamesEngineeringBase::Window& canvas);

bool firstWord(std::string& line, const char* word);
void parseCSV(std::string line, int* output, int tiles);

constexpr unsigned int bulletSize = 1000; // For initialising arrays with specific memory sizes. 
constexpr unsigned int enemySize = 1000;
constexpr unsigned int tileNum = 24; // Number of tiles in the files
constexpr unsigned int worldMaxW = 256; // So the world doesn't get bigger (We need a constant value to initialise arrays)
constexpr unsigned int worldMaxH = 256;
extern int score; // Track the player's score
extern int mode; // Track the mode (infinite or finite map)