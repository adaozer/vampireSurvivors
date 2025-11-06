#include "Tiles.h"
#include "Camera.h"
#include <iostream>

Tile::Tile() {}

bool Tile::load(std::string filename) {
    loaded = image.load(filename);
    if (!loaded) {
        std::cerr << "[Tile] Failed to load: " << filename << "\n";
    }
    return loaded;
}

void Tile::draw(GamesEngineeringBase::Window& canvas, int x, int y, Camera& cam) {
    if (!loaded) return; // guard crash if missing image

    int dx = x - static_cast<int>(cam.getX());
    int dy = y - static_cast<int>(cam.getY());

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
    for (unsigned int i = 0; i < tileNum; ++i) {
        const std::string filename = "Resources/" + pre + std::to_string(i) + ".png";
        if (!t[i].load(filename)) {
            std::cerr << "[TileSet] Missing tile " << i << " -> " << filename << "\n";
        }
    }
}

Tile& TileSet::operator[](unsigned int index) { return t[index]; }
