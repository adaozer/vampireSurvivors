#pragma once
#include "Helper.h"
#include "World.h"

class Player;
class Melee;
class Ranged;
class Camera;

class EnemyManager {
public:
    float timeElapsed = 0.f; // To spawn enemies
    Melee* enemyarr[enemySize]; // Arrays for the 2 enemy types
    Ranged* rangedarr[enemySize];
    float createThreshold = 2.f; // This is how often we'll be spawning enemies (will go down slowly)

    EnemyManager();
    Position determineEnemyPos(Camera& cam, GamesEngineeringBase::Window& canvas);
    void spawnEnemy(GamesEngineeringBase::Window& canvas, Camera& cam, World& w);
    void update(GamesEngineeringBase::Window& canvas, float dt, Player& p, Camera& cam, World& w);
};