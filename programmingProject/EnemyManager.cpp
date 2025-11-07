#include "EnemyManager.h"
#include "Player.h"
#include "Melee.h"
#include "Ranged.h"

EnemyManager::EnemyManager() {
    for (int i = 0; i < enemySize; i++) {
        enemyarr[i] = nullptr;
        rangedarr[i] = nullptr;
    }
}

Position EnemyManager::determineEnemyPos(Camera& cam, GamesEngineeringBase::Window& canvas) {
    Position positions; // Struct that is just int x, int y
    int posX, posY;
    int side = rand() % 4; // Use a switch statement here, check which side of the player we want to spawn
    switch (side) {
    case 0:
        posX = cam.getX() + canvas.getWidth() + 25; // Camera's x and y point to the top left of the screen. We add 25 to all these values to make them off screen
        posY = cam.getY() + (rand() % canvas.getHeight()); // Pick a random y value based on canvas height so we don't spawn every enemy at the same height
        break;
    case 1: // Same things for the other directions
        posX = cam.getX() - 25;
        posY = cam.getY() + (rand() % canvas.getHeight());
        break;
    case 2:
        posX = cam.getX() + (rand() % canvas.getWidth());
        posY = cam.getY() - 25;
        break;
    case 3:
        posX = cam.getX() + (rand() % canvas.getWidth());
        posY = cam.getY() + canvas.getHeight() + 25;
        break;
    }
    positions.x = posX;
    positions.y = posY;
    return positions;
}

void EnemyManager::spawnEnemy(GamesEngineeringBase::Window& canvas, Camera& cam, World& w) {
    if (timeElapsed <= createThreshold) return;
    // Randomly pick a position
    Position pos = determineEnemyPos(cam, canvas);
    int posX = pos.x;
    int posY = pos.y;
    int enemyType = rand() % 5; // Pick which type of enemy to spawn. Case 4 is a ranged enemy the rest are melee enemies with different stats
    int emptySlot = -1;
    if (enemyType == 4) {
        for (int i = 0; i < enemySize; i++)
            if (rangedarr[i] == nullptr) {
                emptySlot = i;
                break;
            }
    }
    else {
        for (int j = 0; j < enemySize; j++)
            if (enemyarr[j] == nullptr) {
                emptySlot = j;
                break;
            }
    } // Pick a slot, important to do here as we have 2 arrays
    if (emptySlot == -1) return;
    switch (enemyType) {
    case 0: // Switch statement to pick which enemy to spawn
        enemyarr[emptySlot] = new Melee(posX, posY, "Resources/enemy1.png", 10, 4, 3);
        break;
    case 1:
        enemyarr[emptySlot] = new Melee(posX, posY, "Resources/enemy2.png", 15, 2, 6);
        break;
    case 2:
        enemyarr[emptySlot] = new Melee(posX, posY, "Resources/enemy3.png", 25, 3, 10);
        break;
    case 3:
        enemyarr[emptySlot] = new Melee(posX, posY, "Resources/enemy4.png", 50, 1, 15);
        break;
    case 4:
        while (!w.inBounds(posX, posY)) {
            // Melee enemies spawning out of bounds is fine as they'll come to the player. However ranged enemies can't move and the player can't shoot what they can't see.
            // Because of this, we keep picking a new random position until we find one that is in the map bounds (but not in the camera bounds)
            Position newPos = determineEnemyPos(cam, canvas);
            posX = newPos.x;
            posY = newPos.y;
        }
            rangedarr[emptySlot] = new Ranged(posX, posY, "Resources/rangedEnemy.png", 10, 0, 25);
            break;
        }
        timeElapsed = 0.f; // Make spawning faster every time we spawn an enemy in
        createThreshold -= 0.1f;
        createThreshold = max(createThreshold, 0.4f); // Cap it at 0.4 so it doesn't go negative (or realllly fast)
    }

void EnemyManager::update(GamesEngineeringBase::Window& canvas, float dt, Player& p, Camera& cam, World& w) {
    timeElapsed += dt;
    spawnEnemy(canvas, cam, w); // Spawn an enemy every x seconds

    for (unsigned int i = 0; i < enemySize; ++i) {
        Melee* e = enemyarr[i];
        if (!e || !e->isAlive()) continue;
        e->move(p, dt);
        e->draw(canvas, cam);
    }
    // Update every melee enemy. Get the player's x and y and keep them moving toward the player
    int pcx = p.getX() + p.image.width / 2;
    int pcy = p.getY() + p.image.height / 2;
    int pr = p.image.width / 3;

    // Check for collision here
    for (unsigned int i = 0; i < enemySize; ++i) {
        Melee* e = enemyarr[i];
        if (!e || !e->isAlive()) continue;

        int ecx = e->getX() + e->image.width / 2;
        int ecy = e->getY() + e->image.height / 2;
        int er = e->image.width / 3;

        if (circlesIntersect(ecx, ecy, er, pcx, pcy, pr)) {
            p.takeDamage(e->damage); // Player takes damage if there is collision
        }
    }

    // If an enemy dies, delete them and increase the player's score
    for (unsigned int i = 0; i < enemySize; ++i) {
        Melee* e = enemyarr[i];
        if (!e) continue;

        if (!e->isAlive()) {
            score++;
            std::cout << "Score: " << score << std::endl;
            delete e;
            enemyarr[i] = nullptr;
            continue;
        }
    }

    // Update ranged enemies. Make them shoot toward the player's position
    for (unsigned int i = 0; i < enemySize; ++i) {
        Ranged* r = rangedarr[i];
        if (!r || !r->isAlive()) continue;
        r->update(dt, p, canvas, cam);
        r->draw(canvas, cam);
    }

    // If a ranged enemy dies, delete it and increase score by 1.
    for (unsigned int i = 0; i < enemySize; ++i) {
        Ranged* r = rangedarr[i];
        if (!r) continue;

        if (!r->isAlive()) {
            score++;
            std::cout << "Score: " << score << std::endl;
            delete r;
            rangedarr[i] = nullptr;
            continue;
        }
    }

}