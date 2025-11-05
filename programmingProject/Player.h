#pragma once
#include <iostream>
#include "Camera.h"
#include "Character.h"
#include "Bullet.h"
#include "Melee.h"
#include "Ranged.h"
#include "Globals.h"

class Player : public Character {
public:
    
    float timeElapsed = 0.f; // To check if enough time has passed to shoot a bullet
    Bullet* barr[bulletSize]; // Bullet array we'll use to store our bullets we shoot with auto attack

    float shootSpeed = 1.f; // How often the player shoots
    float powerupTime = 0.f; // To track how long the powerup has been active
    bool invul = false; // To check if the player is invulnerable (used after getting hit for a brief moment)
    float invulTime = 0.f; // To track how long the player has been invulnerable

    bool powerup = false; // Check if powerup is active
    float powerupCD = 20.f; // Cooldown of the powerup so its not spammed
    float powerupCDTim = 0.f; // Check how long the powerup has been on cooldown
    float powerupLength = 10.f; // To set powerup time to later

    float aoeCD = 10.f; // AOE Attack cooldown and timer to keep track
    float aoeCDTim = 0.f;

    Player(float _posX, float _posY, std::string filepath, int _health, int _speed, int _damage);

    int findNearestEnemyIndex(Melee** enemies, int count, Ranged** renemies, int rangedCount, bool& pickRanged);

    static bool alreadyPicked(Character* c, Character** out, int used) {
        for (int i = 0; i < used; ++i) {
            if (out[i] == c) return true;
        }
        return false;
    }

    int findTopNMaxHealth(Melee** enemies, int enemyCount, Ranged** renemies, int rangedCount, Character** out, int N) {
        int used = 0;

        for (int picked = 0; picked < N; ++picked) {
            Character* best = nullptr;
            int highestHP = -1;

            // scan melee
            for (int i = 0; i < enemyCount; ++i) {
                Melee* e = enemies[i];
                if (!e) continue;
                if (!e->isAlive()) continue;
                if (alreadyPicked(e, out, used)) continue;

                int hp = e->getHealth();
                if (hp > highestHP) {
                    highestHP = hp;
                    best = e;
                }
            }

            // scan ranged
            for (int j = 0; j < rangedCount; ++j) {
                Ranged* r = renemies[j];
                if (!r) continue;
                if (!r->isAlive()) continue;
                if (alreadyPicked(r, out, used)) continue;

                int hp = r->getHealth();
                if (hp > highestHP) {
                    highestHP = hp;
                    best = r;
                }
            }

            // no more enemies to pick
            if (!best)
                break;

            out[used++] = best;
        }

        return used;
    }

    void activatePowerup();

    void takeDamage(int dmg);

    void draw(GamesEngineeringBase::Window& canvas, Camera& cam);

    void updateBullets(float dt, GamesEngineeringBase::Window& canvas, Camera& cam) {
        for (unsigned int i = 0; i < bulletSize; ++i) {
            Bullet* b = barr[i];
            if (!b) continue;
            b->update(dt, *this);

            if (!b->alive) {
                delete b;
                barr[i] = nullptr;
                continue;
            }
            b->draw(canvas, cam);
        }
    }

    bool onScreen(float x, float y, int w, int h, Camera& cam, GamesEngineeringBase::Window& canvas) {
        float sx = x - cam.getX(), sy = y - cam.getY();
        return !(sx + w < 0 || sy + h < 0 || sx > canvas.getWidth() || sy > canvas.getHeight());
    }
    void castAOE(Melee** enemies, int enemyCount, Ranged** renemies, int rangedCount, int baseN, Camera& cam, GamesEngineeringBase::Window& canvas)
    {
        if (aoeCDTim > 0.f) return;
        int N = baseN;
        if (powerup) {
            N += 2;
        }
        if (N > 16) N = 16;
        Melee* onMelee[enemySize];  int onM = 0;
        Ranged* onRng[enemySize];   int onR = 0;

        for (int i = 0; i < enemyCount; ++i) {
            Melee* e = enemies[i];
            if (e && e->isAlive() &&
                onScreen(e->getX(), e->getY(), e->image.width, e->image.height, cam, canvas))
                onMelee[onM++] = e;
        }
        for (int i = 0; i < rangedCount; ++i) {
            Ranged* r = renemies[i];
            if (r && r->isAlive() &&
                onScreen(r->getX(), r->getY(), r->image.width, r->image.height, cam, canvas))
                onRng[onR++] = r;
        }
        Character* targets[16];
        int found = findTopNMaxHealth(onMelee, onM, onRng, onR, targets, N);

        for (int i = 0; i < found; i++) {
            targets[i]->takeDamage(damage * 2);
        }
        aoeCDTim = aoeCD;
    }

    void autoAttack(Melee** enemies, int count, Ranged** renemies, int rangedCount);

    void checkBulletEnemyCollision(Melee** enemies, int enemyCount, Ranged** ranged, int rangedCount);

    void update(float dt, int x, int y);
};