#include "Tiles.h"
#include "Camera.h"
#include <iostream>

Tile::Tile() {}

void Tile::load(std::string filename) {
    image.load(filename); // Load the tile image
}

void Tile::draw(GamesEngineeringBase::Window& canvas, int x, int y, Camera& cam) {
// Standard draw function
    int dx = x - (int)cam.getX();
    int dy = y - (int)cam.getY();

    for (int i = 0; i < image.width; ++i) 
        if (dx+i >= 0 && dx+i < canvas.getWidth()) // >= 0 instead of > 0. Doing > 0 makes the top row have weird dead pixels
            for (int j = 0; j < image.height; ++j)
                if (dy+j >= 0 && dy+j < canvas.getHeight())
                    canvas.draw(dx+i, dy+j, image.atUnchecked(i, j));
}

TileSet::TileSet(std::string pre) { // Load the tileset. We know there are 24 tiles so we load 0.png to 23.png.
    for (int i = 0; i < tileNum; i++) {
        std::string filename = "Resources/" + pre + std::to_string(i) + ".png";
        t[i].load(filename);
    }
}

Tile& TileSet::operator[](unsigned int index) { return t[index]; } // An operator to access each tile individually in a tileset
