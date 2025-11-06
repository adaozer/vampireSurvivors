#pragma once
#include "Tiles.h"
#include <iostream>
#include <sstream>

class Camera;

class World {
public:
    int map[worldMaxH][worldMaxW];
    int tileswide = 0, tileshigh = 0, tilewidth = 0, tileheight = 0;

    World();

    bool loadUsingFile(const std::string& filename);

    void draw(GamesEngineeringBase::Window& canvas, Camera& cam, TileSet& tiles);

    int getWorldWidth();
    int getWorldHeight();

    bool inBoundsIndex(int tx, int ty);
    bool inBounds(int x, int y);
    int tileOperator(int tx, int ty);

};



       
