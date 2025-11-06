#pragma once
#include <fstream>
#include <sstream>
#include "GamesEngineeringBase.h"

class Camera;
struct Position {
	int x, y;
};
bool circlesIntersect(int x1, int y1, int r1, int x2, int y2, int r2);

bool onScreen(float x, float y, float w, float h, Camera& cam, const GamesEngineeringBase::Window& canvas);

std::string clearSpaces(std::string &s);
bool isEmpty(std::string& s);
bool firstWord(std::string& line, const char* word);
bool parseCSV(std::string line, int* output, int tiles);

constexpr unsigned int bulletSize = 1000;
constexpr unsigned int enemySize = 1000;
constexpr unsigned int tileNum = 24;
constexpr unsigned int worldMaxW = 256;
constexpr unsigned int worldMaxH = 256;
extern int score;