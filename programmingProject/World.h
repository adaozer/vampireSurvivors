#pragma once
#include "Tiles.h"
#include <iostream>
#include <sstream>

class Camera;

class World {
public:
    int map[worldMaxH][worldMaxW]; // Map array. Stores both X and Y coordinates so map[2][1] would give the first tile of the second row (map[y][x]).
    bool blockedTile[worldMaxH][worldMaxW]; // Same thing but this time map[2][1] would give whether the first tile of the second row is passable or not (water or not).
    int tileswide = 0, tileshigh = 0, tilewidth = 0, tileheight = 0; // We will read these from the text file

    World();

    void loadUsingFile(const std::string& filename);

    void draw(GamesEngineeringBase::Window& canvas, Camera& cam, TileSet& tiles);

    void drawInfinite(GamesEngineeringBase::Window& canvas, Camera& cam, TileSet& tiles);

    int getWorldWidth();
    int getWorldHeight();

    bool isWalkablePixel(int px, int py);
    bool isWalkableRect(int x, int y, int w, int h);
    bool isWalkableInfinite(int x, int y, int w, int h);
    void collisionLayer();

    bool inBounds(int x, int y);

    int tileAtInfinite(int tx, int ty);
};



       
