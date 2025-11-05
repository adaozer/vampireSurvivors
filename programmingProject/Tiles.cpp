#include "Tiles.h"

Tile::Tile() {}

void Tile::load(std::string filename) {
	image.load(filename);
}

void Tile::draw(GamesEngineeringBase::Window &canvas, int x, int y, Camera &cam) {
	int dx = x - (int)cam.getX();
	int dy = y - (int)cam.getY();
	for (int i = 0; i < image.width; i++)
		if (dx + i > 0 && dx + i < canvas.getWidth())
			for (int j = 0; j < image.height; j++)
				if (dy + j > 0 && dy + j < canvas.getHeight())
					canvas.draw(dx + i, dy + j, image.atUnchecked(i, j));
}

int Tile::getHeight() { return image.height; }
int Tile::getWidth() { return image.width; }
GamesEngineeringBase::Image& Tile::getSprite() { return image; }

TileSet::TileSet(std::string pre) {
	for (unsigned int i = 0; i < tileNum; i++) {
		std::string filename;
		filename = "Resources/" + pre + std::to_string(i) + ".png";
		t[i].load(filename);
	}
}

Tile& TileSet::operator[](unsigned int index) { return t[index]; }