#include "Tiles.h"
#include "Camera.h"
#include <iostream>

Tile::Tile() {}

void Tile::load(std::string filename) {
    image.load(filename);
}

void Tile::draw(GamesEngineeringBase::Window& canvas, int x, int y, Camera& cam) {
    int dx = x - (int)cam.getX();
    int dy = y - (int)cam.getY();

    for (int i = 0; i < image.width; ++i) {
        int cx = dx + i;
        if (cx < 0 || cx >= canvas.getWidth()) continue;

        for (int j = 0; j < image.height; ++j) {
            int cy = dy + j;
            if (cy < 0 || cy >= canvas.getHeight()) continue;

            canvas.draw(cx, cy, image.atUnchecked(i, j));
        }
    }
}

int Tile::getHeight() { return image.height; }
int Tile::getWidth() { return image.width; }
GamesEngineeringBase::Image& Tile::getSprite() { return image; }

TileSet::TileSet(std::string pre) {
    for (int i = 0; i < tileNum; i++) {
        std::string filename = "Resources/" + pre + std::to_string(i) + ".png";
        t[i].load(filename);
    }
}

Tile& TileSet::operator[](unsigned int index) { return t[index]; }
