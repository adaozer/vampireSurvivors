#pragma once
#include "Helper.h"
#include "World.h"

class Player;
class Melee;
class Ranged;
class Camera;

class EnemyManager {
public:
    float timeElapsed = 0.f;
    Melee* enemyarr[enemySize];
    Ranged* rangedarr[enemySize];
    float createThreshold = 2.f;

    EnemyManager();
    Position determineEnemyPos(Player& p);
    void spawnEnemy(GamesEngineeringBase::Window& canvas, Player& p, World& w);
    void update(GamesEngineeringBase::Window& canvas, float dt, Player& p, Camera& cam, World& w);
};