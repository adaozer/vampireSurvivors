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

Position EnemyManager::determineEnemyPos(Player& p) {
    Position positions;
    int posX, posY;
    int side = rand() % 4;
    switch (side) {
    case 0:
        posX = p.getX() + 537;
        posY = p.getY() + ((rand() % 768) - 384);
        break;
    case 1:
        posX = p.getX() - 537;
        posY = p.getY() + ((rand() % 768) - 384);
        break;
    case 2:
        posX = p.getX() + ((rand() % 1024) - 512);
        posY = p.getY() - 409;
        break;
    case 3:
        posX = p.getX() + ((rand() % 1024) - 512);
        posY = p.getY() + 409;
        break;
    }
    positions.x = posX;
    positions.y = posY;
    return positions;
}

void EnemyManager::spawnEnemy(GamesEngineeringBase::Window& canvas, Player& p, World& w) {
    if (timeElapsed <= createThreshold) return;
    Position pos = determineEnemyPos(p);
    int posX = pos.x;
    int posY = pos.y;
    int enemyType = rand() % 5;
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
    }
    if (emptySlot == -1) return;
    switch (enemyType) {
    case 0:
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
            Position newPos = determineEnemyPos(p);
            posX = newPos.x;
            posY = newPos.y;
        }
            rangedarr[emptySlot] = new Ranged(posX, posY, "Resources/rangedEnemy.png", 10, 0, 25);
            break;
        }
        timeElapsed = 0.f;
        createThreshold -= 0.1f;
        createThreshold = max(createThreshold, 0.35f);
    }

void EnemyManager::update(GamesEngineeringBase::Window& canvas, float dt, Player& p, Camera& cam, World& w) {
    timeElapsed += dt;
    spawnEnemy(canvas, p, w);

    for (unsigned int i = 0; i < enemySize; ++i) {
        Melee* e = enemyarr[i];
        if (!e || !e->isAlive()) continue;
        e->move(p, dt);
        e->draw(canvas, cam);
    }

    int pcx = (int)p.getX() + p.image.width / 2;
    int pcy = (int)p.getY() + p.image.height / 2;
    int pr = p.image.width / 3;

    for (unsigned int i = 0; i < enemySize; ++i) {
        Melee* e = enemyarr[i];
        if (!e || !e->isAlive()) continue;

        int ecx = (int)e->getX() + e->image.width / 2;
        int ecy = (int)e->getY() + e->image.height / 2;
        int er = e->image.width / 3;

        if (circlesIntersect(ecx, ecy, er, pcx, pcy, pr)) {
            p.takeDamage(e->damage);
        }
    }

    for (unsigned int i = 0; i < enemySize; ++i) {
        Melee* e = enemyarr[i];
        if (!e) continue;

        if (!e->isAlive()) {
            score++;
            delete e;
            enemyarr[i] = nullptr;
            continue;
        }
    }

    for (unsigned int i = 0; i < enemySize; ++i) {
        Ranged* r = rangedarr[i];
        if (!r || !r->isAlive()) continue;
        r->update(dt, p, canvas, cam);
        r->draw(canvas, cam);
    }

    for (unsigned int i = 0; i < enemySize; ++i) {
        Ranged* r = rangedarr[i];
        if (!r) continue;

        if (!r->isAlive()) {
            score++;
            delete r;
            rangedarr[i] = nullptr;
            continue;
        }
    }

}