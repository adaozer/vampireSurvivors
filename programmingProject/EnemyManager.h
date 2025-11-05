#pragma once
#include "Player.h"

class EnemyManager {
public:
    float timeElapsed = 0.f;
    Melee* enemyarr[enemySize];
    Ranged* rangedarr[enemySize];
    unsigned int currentSize = 0;
    float createThreshold = 2.f;
    int rangedCount = 0;

    EnemyManager();
    Position determineEnemyPos(Player& p);
    void spawnEnemy(GamesEngineeringBase::Window& canvas, Player& p);
    void update(GamesEngineeringBase::Window& canvas, float dt, Player& p, Camera& cam);
};