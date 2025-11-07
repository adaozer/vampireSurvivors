#include "World.h"
#include "Camera.h"

World::World() {
	for (int y = 0; y < worldMaxH; y++)
		for (int x = 0; x < worldMaxW; x++) {
			map[y][x] = 0;
		}
}

bool World::loadUsingFile(const std::string& filename) {
    std::ifstream f(filename);
    if (!f.good()) return false;

    int W = -1, H = -1, tw = -1, th = -1;
    bool inLayer = false;
    unsigned rowsRead = 0;

    std::string line;
    while (std::getline(f, line)) {
        if (isEmpty(line)) continue;

        if (!inLayer) {
            std::string s = clearSpaces(line);

            if (firstWord(s, "tileswide")) {
                std::istringstream iss(s);
                std::string key; iss >> key >> W;
                continue;
            }
            if (firstWord(s, "tileshigh")) {
                std::istringstream iss(s);
                std::string key; iss >> key >> H;
                continue;
            }
            if (firstWord(s, "tilewidth")) {
                std::istringstream iss(s);
                std::string key; iss >> key >> tw;
                continue;
            }
            if (firstWord(s, "tileheight")) {
                std::istringstream iss(s);
                std::string key; iss >> key >> th;
                continue;
            }
            if (firstWord(s, "layer")) {
                // Sanity checks before reading grid
                if (W <= 0 || H <= 0) return false;
                if (W > static_cast<int>(worldMaxW) || H > static_cast<int>(worldMaxH)) return false;
                if (tw <= 0 || th <= 0) return false;

                tileswide = W;
                tileshigh = H;
                tilewidth = tw;
                tileheight = th;

                inLayer = true;
                rowsRead = 0;
                continue;
            }

            continue;
        }

        // Layer phase: read exactly `height` rows with exactly `width` ints each.
        if (rowsRead >= tileshigh) break;

        int vals[worldMaxW];
        if (!parseCSV(line, vals, static_cast<int>(tileswide))) {
            // Allow blank/comment lines inside the layer (skip), else fail
            if (isEmpty(line)) continue;
            return false;
        }

        // Clamp to valid tile ID range [0, tileNum-1]
        for (unsigned x = 0; x < tileswide; ++x) {
            int v = vals[x];
            if (v < 0) v = 0;
            if (v >= static_cast<int>(tileNum)) v = static_cast<int>(tileNum) - 1;
            map[rowsRead][x] = v;
        }
        ++rowsRead;
    }

    // Success only if we actually entered layer data and read all rows.
    return (inLayer && rowsRead == tileshigh);
}

void World::draw(GamesEngineeringBase::Window& canvas, Camera& cam, TileSet& tiles)
{
    if (tileswide == 0 || tileshigh == 0 || tilewidth == 0 || tileheight == 0) return;

    const int camX = (int)cam.getX();
    const int camY = (int)cam.getY();
    const int screenW = (int)canvas.getWidth();
    const int screenH = (int)canvas.getHeight();

    // start with truncating division
    int minTileX = camX / tilewidth;
    int minTileY = camY / tileheight;

    // adjust for negatives to emulate floor division
    if (camX < 0 && (camX % tilewidth))  --minTileX;
    if (camY < 0 && (camY % tileheight)) --minTileY;

    // ceil for the far edge
    int maxTileX = (camX + screenW + tilewidth - 1) / tilewidth;
    int maxTileY = (camY + screenH + tileheight - 1) / tileheight;

    // clamp to map bounds
    if (minTileX < 0) minTileX = 0;
    if (minTileY < 0) minTileY = 0;
    if (maxTileX > tileswide)  maxTileX = tileswide;
    if (maxTileY > tileshigh)  maxTileY = tileshigh;

    for (int ty = minTileY; ty < maxTileY; ++ty) {
        for (int tx = minTileX; tx < maxTileX; ++tx) {
            const int id = map[ty][tx];
            const int worldX = tx * tilewidth;
            const int worldY = ty * tileheight;
            tiles[(unsigned)id].draw(canvas, worldX, worldY, cam);
        }
    }
}

void World::buildCollisionLayer()
{
    for (int ty = 0; ty < tileshigh; ++ty) {
        for (int tx = 0; tx < tileswide; ++tx) {
            int id = map[ty][tx];

            // Mark water tiles (IDs 14–22) as blocked
            // You can change this range if your tileset differs
            blockedCell[ty][tx] = (id >= 14 && id <= 22);
        }
    }
}

bool World::isWalkablePixel(int px, int py)
{
    if (!inBounds(px, py)) return false;

    int tx = px / tilewidth;
    int ty = py / tileheight;

    if (tx < 0 || ty < 0 || tx >= tileswide || ty >= tileshigh)
        return false;

    // true means blocked, so we return the opposite
    return !blockedCell[ty][tx];
}

bool World::isWalkableRect(int x, int y, int w, int h)
{
    // Check 4 corners of the player's bounding box
    return isWalkablePixel(x, y) &&
        isWalkablePixel(x + w - 1, y) &&
        isWalkablePixel(x, y + h - 1) &&
        isWalkablePixel(x + w - 1, y + h - 1);
}

int World::getWorldWidth() { return tileswide * tilewidth; }
int World::getWorldHeight() { return tileshigh * tileheight; }

bool World::inBoundsIndex(int tx, int ty) {
    if (tx >= 0 && ty >= 0 && tx < tileswide && ty < tileshigh) return true;
    return false;
}

bool World::inBounds(int x, int y) {
    if (x >= 0 && y >= 0 && x < getWorldWidth() && y < getWorldHeight()) return true;
    return false;
}

int World::tileOperator(int tx, int ty) {
    if (!inBoundsIndex(tx, ty)) return -1;
    return map[ty][tx];
}