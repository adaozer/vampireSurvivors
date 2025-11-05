#pragma once

struct Position {
	int x, y;
};
bool circlesIntersect(int x1, int y1, int r1, int x2, int y2, int r2);

constexpr unsigned int bulletSize = 1000;
constexpr unsigned int enemySize = 1000;
extern int score;