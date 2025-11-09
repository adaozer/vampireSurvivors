#include "World.h"
#include "Camera.h"

World::World() { // Initialise map  array
	for (int y = 0; y < worldMaxH; y++)
		for (int x = 0; x < worldMaxW; x++) {
			map[y][x] = 0;
		}
}

void World::loadUsingFile(const std::string& filename) {
    std::ifstream f(filename); // Open the file for reading

    bool inLayer = false;  // Whether we're reading the part that starts after "Layer 0", AKA the tile and map width height info is done
    int rowsRead = 0; // Row read tracker to make sure we read the amount of rows specified by tilehigh.

    std::string line; // Read line by line
    while (std::getline(f, line)) { // While there are lines to read
        if (line.empty()) continue; // Skip empty lines

        if (!inLayer) { // Before the layers start we get the variable information
            if (firstWord(line, "tileswide")) { // Check if the first word is X (tileswide here)
                std::istringstream iss(line); // Use an istringstream to read it easily
                std::string key; 
                iss >> key >> tileswide; // First we read the key (tileswide) then the value (42)
                continue;
            }
            if (firstWord(line, "tileshigh")) { // Repeat for everything else
                std::istringstream iss(line);
                std::string key; iss >> key >> tileshigh;
                continue;
            }
            if (firstWord(line, "tilewidth")) {
                std::istringstream iss(line);
                std::string key; iss >> key >> tilewidth;
                continue;
            }
            if (firstWord(line, "tileheight")) {
                std::istringstream iss(line);
                std::string key; iss >> key >> tileheight;
                continue;
            }
            if (firstWord(line, "layer")) { // The layer starts
                inLayer = true;
                continue;
            }
        }

        if (rowsRead >= tileshigh) break; // Make sure we read the right amount of rows

        int vals[worldMaxW];
        parseCSV(line, vals, tileswide); // Parse the csv, add the CSV values to vals.

        // Parse the rows and put the values in vals into maps at the correct positions
        for (int i = 0; i < tileswide; i++) {
            int v = vals[i];
            map[rowsRead][i] = v; // Row, value format
        }
        rowsRead++; // Increment the rows read after a rows been read
    }

    if (rowsRead == tileshigh) return; // Return after all the rows have been read
}

void World::draw(GamesEngineeringBase::Window& canvas, Camera& cam, TileSet& tileset) {
    if (tileswide == 0 || tileshigh == 0 || tilewidth == 0 || tileheight == 0) return; // Quick check in case we haven't read the values correctly

    int minTileX = cam.getX() / tilewidth; // Divide the cam's X (top left of the screen) by 32, and we get the index of the leftmost tile
    int minTileY = cam.getY() / tileheight; // Same for the upmost tile

    // This is to fix the result of the division in case of floats and negative coordinates. Basically we need // (floor) division
    if (cam.getX() < 0 && ((int)cam.getX() % tilewidth))  minTileX--;
    if (cam.getY() < 0 && ((int)cam.getY() % tileheight)) minTileY--; 

    // We do the exact same thing for the index of the rightmost/bottommost tile.
    int maxTileX = (cam.getX() + canvas.getWidth() + tilewidth - 1) / tilewidth; // Without the -1's here weird pixels form on the edges of the screen
    int maxTileY = (cam.getY() + canvas.getHeight() + tileheight - 1) / tileheight;

    // Some clamping
    if (minTileX < 0) minTileX = 0;
    if (minTileY < 0) minTileY = 0;
    if (maxTileX > tileswide) maxTileX = tileswide;
    if (maxTileY > tileshigh) maxTileY = tileshigh;

    // Simple loop going from the minimum tile's index to the maximum tile's index
    for (int ty = minTileY; ty < maxTileY; ty++) {
        for (int tx = minTileX; tx < maxTileX; tx++) {
            int id = map[ty][tx]; // Select tile id from the map[y][x] array
            int worldX = tx * tilewidth; // Index of the tile * 32 pixels to find out its coordinates
            int worldY = ty * tileheight;
            tileset[id].draw(canvas, worldX, worldY, cam); // Regular draw
        }
    }
}

void World::drawInfinite(GamesEngineeringBase::Window& canvas, Camera& cam, TileSet& tileset) {
    if (tileswide == 0 || tileshigh == 0 || tilewidth == 0 || tileheight == 0) return;  // Quick check in case we haven't read the values correctly

    // Find leftmost tile just like regular draw
    int minTileX = cam.getX() / tilewidth;
    if (cam.getX() < 0 && ((int)cam.getX() % tilewidth)) minTileX--; // This is to make it like // division 

    // This is for the topmost tile
    int minTileY = cam.getY() / tileheight;
    if (cam.getY() < 0 && ((int)cam.getY() % tileheight)) minTileY--;

    // This is the topright tile
    int maxTileX = (cam.getX() + canvas.getWidth() + tilewidth - 1) / tilewidth;
    if ((cam.getX() + canvas.getWidth() + tilewidth - 1) < 0 && (((int)(cam.getX() + canvas.getWidth() + tilewidth - 1)) % tilewidth)) maxTileX--;


    // This is the bottomright tile
    int maxTileY = (cam.getY() + canvas.getHeight() + tileheight - 1) / tileheight;
    if ((cam.getY() + canvas.getHeight() + tileheight - 1) < 0 && (((int)(cam.getY() + canvas.getHeight() + tileheight - 1)) % tileheight)) maxTileY--;
 

    // Regular draw
    for (int ty = minTileY; ty <= maxTileY; ty++) {
        for (int tx = minTileX; tx <= maxTileX; tx++) {
            int id = tileAtInfinite(tx, ty); // Instead of calling map[ty][tx] we call tileatInfinite which is a function that "normalises" the tile indices with the % operator
            tileset[id].draw(canvas, tx * tilewidth, ty * tileheight, cam);
        }
    }
}

void World::collisionLayer() {
    // Build the collision map
    for (int y = 0; y < tileshigh; y++) {
        for (int x = 0; x < tileswide; x++) {
            int tileNo = map[y][x]; // Check the number of the tile at that spot
            blockedTile[y][x] = (tileNo >= 14 && tileNo <= 22); // If the tile is 14 - 22 its blocked so put true in the blocked cell array
            // Its very hard coded, it works because tiles 14 to 22 on the tile pngs are water.
        }
    }
}

bool World::isWalkablePixel(int px, int py) {
    // Helper function for "isWalkableRect"
    int tx = px / tilewidth; // Find out the index of the tile we are on. We know each tile is 32x32, so by dividing pixel's X with 32 we can get the index we are on
    int ty = py / tileheight; // Same for the Y value

    return !blockedTile[ty][tx]; // Reference the index of our tile with the blocked cell array. True = Blocked so we return the opposite to see if its walkable
}

bool World::isWalkableRect(int x, int y, int w, int h) {
    // x and y are the player's x and y, w and h are the player's sprite's width and height.
    // First, the actual x and y of the player (top left corner), then the top right corner (add the width of the sprite to the top left value (x).
    // Then, the bottom left of the sprite (add the height of the sprite to the Y) then finally the bottom right (add sprite's width and height to X and Y)
    // The sprite is 32x32 and so are the tiles so we can get accurate information here. Also, the -1's are required as otherwise it spills into the next tile (0, 31 for each tile)
    return isWalkablePixel(x, y) && isWalkablePixel(x + w - 1, y) && isWalkablePixel(x, y + h - 1) && isWalkablePixel(x + w - 1, y + h - 1);
} 

bool World::isWalkableInfinite(int x, int y, int w, int h) {
    if (tileswide == 0 || tileshigh == 0 || tilewidth == 0 || tileheight == 0)
        return true;

    // Corner pixels
    int px1 = x;             int py1 = y;              // TL
    int px2 = x + w - 1;     int py2 = y;              // TR
    int px3 = x;             int py3 = y + h - 1;      // BL
    int px4 = x + w - 1;     int py4 = y + h - 1;      // BR

    // Convert to tile indices with FLOOR division (not trunc toward zero)
    int tx1 = px1 / tilewidth;    if (px1 < 0 && (px1 % tilewidth)) --tx1;
    int ty1 = py1 / tileheight;   if (py1 < 0 && (py1 % tileheight)) --ty1;

    int tx2 = px2 / tilewidth;    if (px2 < 0 && (px2 % tilewidth)) --tx2;
    int ty2 = py2 / tileheight;   if (py2 < 0 && (py2 % tileheight)) --ty2;

    int tx3 = px3 / tilewidth;    if (px3 < 0 && (px3 % tilewidth)) --tx3;
    int ty3 = py3 / tileheight;   if (py3 < 0 && (py3 % tileheight)) --ty3;

    int tx4 = px4 / tilewidth;    if (px4 < 0 && (px4 % tilewidth)) --tx4;
    int ty4 = py4 / tileheight;   if (py4 < 0 && (py4 % tileheight)) --ty4;

    // Wrap via tileAtInfinite and block on water (14..22)
    int id1 = tileAtInfinite(tx1, ty1); if (id1 >= 14 && id1 <= 22) return false;
    int id2 = tileAtInfinite(tx2, ty2); if (id2 >= 14 && id2 <= 22) return false;
    int id3 = tileAtInfinite(tx3, ty3); if (id3 >= 14 && id3 <= 22) return false;
    int id4 = tileAtInfinite(tx4, ty4); if (id4 >= 14 && id4 <= 22) return false;

    return true;
}

int World::getWorldWidth() { return tileswide * tilewidth; } // Get world width by multiplying the dimensions of tiles and how many tiles there is. In this case this is 42x32.
int World::getWorldHeight() { return tileshigh * tileheight; } // Same for height

bool World::inBounds(int x, int y) {
    // This is a simple in bounds check to use for clamping camera, player movement etc. Checks if x and y are above 0 and the world's width/height.
    if (x >= 0 && y >= 0 && x < getWorldWidth() && y < getWorldHeight()) return true;
    return false;
}

int World::tileAtInfinite(int tx, int ty) {
    // Because the world is repeating, we will have really large tile indices. We % by 42 to get the "normalised" tile index
    int worldX = tx % tileswide;
    int worldY = ty % tileshigh;

    // This is to "normalise" negative indices. We add 42 to make it positive again and "wrap" around.
    if (worldX < 0) worldX += tileswide;
    if (worldY < 0) worldY += tileshigh;

    // Return the value "0.png" etc, by using the proper normalised tile indices
    return map[worldY][worldX];
}
