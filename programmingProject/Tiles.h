#pragma once
#include "Helper.h"

class Camera;

class Tile {
public:
	GamesEngineeringBase::Image image; // Sprite of the tile
	Tile();

	void load(std::string filename);
	void draw(GamesEngineeringBase::Window &canvas, int x, int y, Camera &cam);
};

class TileSet {
public:
	Tile t[tileNum]; // Array of tiles so the tileset can store the tiles
	TileSet(std::string pre = "");

	Tile& operator[](unsigned int index);
};
