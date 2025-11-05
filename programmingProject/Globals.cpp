#include "Globals.h"

bool circlesIntersect(int x1, int y1, int r1, int x2, int y2, int r2)
{
    int dx = x1 - x2;
    int dy = y1 - y2;
    int dist2 = dx * dx + dy * dy;
    int rsum = r1 + r2;
    return dist2 <= rsum * rsum;
}

int score = 0;
