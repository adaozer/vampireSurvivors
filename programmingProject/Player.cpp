#include "Player.h"
#include "Melee.h"
#include "Ranged.h"
#include "Bullet.h"

Player::Player(float _posX, float _posY, std::string filepath, int _health, int _speed, int _damage) : Character(_posX, _posY, filepath, _health, _speed, _damage) {
    for (int i = 0; i < bulletSize; i++) 
        barr[i] = nullptr;
}

Player::~Player() { // Destructor to get rid of the bullet array
    for (int i = 0; i < bulletSize; i++)
        if (barr[i])
            delete barr[i];
}

int Player::findNearestEnemyIndex(Melee** enemies, Ranged** renemies, bool& pickRanged) {
    // Helper function to help us find where the closest enemy is to shoot at automatically
    int closest = 10000000; // Big number to start off
    int closestIndex = -1; // Index
    pickRanged = false;  // Because melee and ranged enemies are stored in seperate arrays, just to keep track

    for (int i = 0; i < enemySize; i++) {
        Melee* e = enemies[i];
        if (!e || !e->isAlive()) continue; // Make sure its a live enemy and not a nullptr or dead

        int dx = e->getX() - posX;
        int dy = e->getY() - posY;
        int distance = (dx * dx) + (dy * dy); // Simple distance between 2 points formula, (x2-x1)**2 + (y2-y1)**2
        // still accurate without square root so we don't calculate it to save performance

        if (distance < closest) {
            closest = distance;
            closestIndex = i;
            pickRanged = false; // since this is the melee loop
        }
    }

    for (int j = 0; j < enemySize; j++) {
        Ranged* r = renemies[j];
        if (!r || !r->isAlive()) continue;

        int dx = r->getX() - posX;
        int dy = r->getY() - posY;
        int dist2 = dx * dx + dy * dy;

        if (dist2 < closest) {
            closest = dist2;
            closestIndex = j;
            pickRanged = true; // same thing but this is the ranged loop
        }
    }

    return closestIndex;
}

void Player::autoAttack(Melee** enemies, Ranged** renemies, Camera& cam, GamesEngineeringBase::Window &canvas) { // Player's default (automatic) attack function
    bool pickRanged = false;
    int i = findNearestEnemyIndex(enemies, renemies, pickRanged); // Find the nearest enemy using the helper function
    if (i == -1) return;

    float towardX, towardY; // The target's X and Y, so we know where to shoot at 
    int targetW, targetH; // The target's X and Y so we shoot at their middle point. Otherwise it looks weird and its also surprisingly hard to aim.

    if (pickRanged) {
        Ranged* r = renemies[i];
        if (!r || !r->isAlive()) return;
        towardX = r->getX();
        towardY = r->getY();
        targetW = r->image.width;
        targetH = r->image.height;
    }
    else {
        Melee* e = enemies[i];
        if (!e || !e->isAlive()) return;
        towardX = e->getX();
        towardY = e->getY();
        targetW = e->image.width;
        targetH = e->image.height;
    } // Get the X, Y, W, H values of the enemy. We use pickRanged to find out which array we need to parse.

    if (!onScreen(towardX, towardY, targetW, targetH, cam, canvas)) return; // If the enemy is not on screen, don't shoot

    float currentSpeed = shootSpeed; // Seperate variable so we don't override shootSpeed
    if (powerup) currentSpeed = 0.25f; // Shoot faster if theres powerup active
    if (timeElapsed < currentSpeed) return;

    for (int i = 0; i < bulletSize; i++) {
        if (barr[i] == nullptr) { // Because we go from 0 to bulletSize and spawn at the first nullptr, we can have bullets spawning infinitely
            barr[i] = new Bullet(posX + image.width / 2, posY + image.height / 2, towardX + targetW / 2, towardY + targetH / 2, true, damage); // Same spawning we used in class
            timeElapsed = 0.f; // Restart the timer to shoot
            return;
        }
    }
}

void Player::activatePowerup() {
    if (powerupCDTim > 0.f) return; // Check if powerup is on cooldown
    powerup = true; // Activate powerup
    powerupTime = powerupLength; // Powerup is poweruplength seconds long 
    powerupCDTim = powerupCD; // Powerup cd is powerupCD seconds long (all of these are set in the class definition)
}

void Player::takeDamage(int dmg) {
    if (invul) return;
    std::cout << "Player took " << dmg << " Damage\n"; // Log the damage
    Character::takeDamage(dmg);  // Simple health - damage operation. Alive = False if health falls below 0.
    invul = true;
    invulTime = 1.0f; // Take damage if the player is not invulerable. Then make the player invulnerable for 1 second.
}

void Player::draw(GamesEngineeringBase::Window& canvas, Camera& cam) {
    int ix = (int)(posX - cam.getX()); // Use player's X - camera's X to draw the player's position properly in relation to the camera
    int iy = (int)(posY - cam.getY()); // Storing posX and posY as floats is better for maths equations but not for GamesEngineeringBase draw, so we turn them back into ints here.
    for (unsigned int k = 0; k < image.height; k++)
        if (k + iy > 0 && k + iy < canvas.getHeight()) {
            for (unsigned int j = 0; j < image.width; j++)
                if (j + ix > 0 && j + ix < canvas.getWidth())
                    if (image.alphaAtUnchecked(j, k) > 0)
                        canvas.draw(ix + j, iy + k, image.atUnchecked(j, k)); // Player sprite draw, same as all the other ones we've done in class
        }
}

void Player::updateBullets(float dt, GamesEngineeringBase::Window& canvas, Camera& cam) {
    // This parses the bullet array to run bullet update on each live bullet and draw them.
    for (unsigned int i = 0; i < bulletSize; ++i) {
        Bullet* b = barr[i];
        if (!b) continue;
        b->update(dt, *this, canvas, cam); // Update bullet

        if (!b->alive) { // If the bullet isn't live, it deletes the bullet.
            delete b;
            barr[i] = nullptr;
            continue;
        }
        b->draw(canvas, cam); // Draw bullet
    }
}

void Player::checkBulletEnemyCollision(Melee** enemies, Ranged** ranged) {
    // This function checks if a player bullet has hit an enemy.
    for (int i = 0; i < bulletSize; i++) {
        Bullet* b = barr[i];
        if (!b || !b->alive) continue;
        // Parse the bullet array and get the radius x and y of the bullet so we can check collision
        int bcx = b->getX();
        int bcy = b->getY();
        int br = b->radius;

        bool hit = false; // This flag is to see if we need to parse both arrays since we have a seperate melee and ranged array
        for (int j = 0; j < enemySize; j++) {
            Melee* e = enemies[j];
            if (!e || !e->isAlive()) continue;

            int ecx = e->getX() + e->image.width / 2;
            int ecy = e->getY() + e->image.height / 2;
            int er = e->image.width / 3;
            // Also get the enemies x, y, radius info so we can check for collision
            if (circlesIntersect(bcx, bcy, br, ecx, ecy, er)) {
                e->takeDamage(b->damage);
                hit = true; // Hit = true, since the bullet will then disappear we don't need to parse ranged array
                break;
            }
        }

        if (!hit) { // If the bullet didn't hit a melee enemy, we also parse the ranged array
            for (int k = 0; k < enemySize; k++) {
                Ranged* r = ranged[k];
                if (!r || !r->isAlive()) continue;
                 // Same procedure
                int rcx = (int)r->getX() + r->image.width / 2;
                int rcy = (int)r->getY() + r->image.height / 2;
                int rr = r->image.width / 3;

                if (circlesIntersect(bcx, bcy, br, rcx, rcy, rr)) {
                    r->takeDamage(b->damage);
                    hit = true;
                    break;
                }
            }
        }

        if (hit) { // If the bullet hit any enemy it gets deleted since its no longer active
            b->alive = false;
            delete b;
            barr[i] = nullptr;
        }
    }
}


bool Player::alreadyPicked(Character* c, Character** charList, int arrSize) {
    // The purpose of this function is to do "if i in list" like in python, to not have duplicates
    for (int i = 0; i < arrSize; ++i) {
        if (charList[i] == c) return true;
    }
    return false;
}

int Player::findTopNMaxHealth(Melee** enemies, int mCount, Ranged** renemies, int rCount, Character** charList, int N) {
    int arrSize = 0;

    // Very simple function that parses both enemy arrays to find the highest health ones. A pointer to charList is passed to modify it and fill it here since we're turning the amount of elements inside
    // First we parse melee then ranged and add the highest health one to the character array. We return array size to know how much to parse

    for (int i = 0; i < N; i++) {
        Character* best = nullptr;
        int highestHP = -1;

        for (int m = 0; m < mCount; m++) {
            Melee* e = enemies[m];
            if (!e || !e->isAlive() || alreadyPicked(e, charList, arrSize)) continue; // already picked is used to not just pick the same enemy 10 times
            int hp = e->getHealth();
            if (hp > highestHP) {
                highestHP = hp;
                best = e;
            }
        }

        for (int j = 0; j < rCount; j++) {
            Ranged* r = renemies[j];
            if (!r || !r->isAlive() || alreadyPicked(r, charList, arrSize)) continue;
            int hp = r->getHealth();
            if (hp > highestHP) {
                highestHP = hp;
                best = r;
            }
        }
        if (!best) break;
        charList[arrSize++] = best;
    }
    return arrSize;
}

void Player::castAOE(Melee** enemies, Ranged** renemies, int N, Camera& cam, GamesEngineeringBase::Window& canvas)
{
    if (aoeCDTim > 0.f) return; // Check cooldown
    if (powerup) { // Shoot double the amount of enemies if powerup is active
        N *= 2;
    }

    // This section uses the onScreen helper function to create arrays of ranged + melee enemies that are actually on screen

    Melee* marr[enemySize];
    int mCount = 0;
    Ranged* rarr[enemySize];
    int rCount = 0;

    // Parse the main enemy arrays and add them to the onscreen arrays 
    for (int i = 0; i < enemySize; i++) {
        Melee* e = enemies[i];
        if (e && e->isAlive() && onScreen(e->getX(), e->getY(), e->image.width, e->image.height, cam, canvas)) {
            marr[mCount] = e;
            mCount++;
        }
    }

    for (int i = 0; i < enemySize; i++) {
        Ranged* r = renemies[i];
        if (r && r->isAlive() && onScreen(r->getX(), r->getY(), r->image.width, r->image.height, cam, canvas)) {
            rarr[rCount] = r;
            rCount++;
        }   
    }

    // Make a character array of 20 spaces (10 or 20 needed based on if powerup is active
    // Character used since theres going to be both melee and ranged classes inside

    Character* targets[20];
    int targetsLength = findTopNMaxHealth(marr, mCount, rarr, rCount, targets, N);
    // Pass targets through N top max health helper function and damage all the enemies inside
    for (int i = 0; i < targetsLength; i++) {
        targets[i]->takeDamage(damage * 2);
    }
    aoeCDTim = aoeCD;
}

void Player::playerMovement(float x, float y, World& w) {
    float initialX = posX; // To make sure that we are not walking on water, we use a try/error type function
    posX += x;

    if (mode == 2) { // Specific water check if the infinite map is selected
        if (!w.isWalkableInfinite(posX, posY, image.width, image.height)) {
            posX = initialX; // If its not walkable, roll back X.
        }
    }
    else {  // If its the finite map, we just do some clamping so we don't go past the tiles.
        if (posX < 0) posX = 0;
        if (posX > w.getWorldWidth() - image.width) posX = w.getWorldWidth() - image.width;

        if (!w.isWalkableRect(posX, posY, image.width, image.height)) {
            posX = initialX; // Specific water tile search function for the finite map, once again we roll back the X if it was water
        }
    }
    // Do the same for Y
    float initialY = posY;
    posY += y;
    // Checking seperately for X and Y makes the movement feel more smooth as checking for X and Y at once means 
    // holding 2 directions at once while moving near water will just make the character completely stop moving instead of just not go in the water
    if (mode == 2) {
        if (!w.isWalkableInfinite(posX, posY, image.width, image.height)) {
            posY = initialY;
        }
    }
    else { // Clamp if finite
        if (posY < 0) posY = 0;
        if (posY > w.getWorldHeight() - image.height) posY = w.getWorldHeight() - image.height;

        if (!w.isWalkableRect(posX, posY, image.width, image.height)) {
            posY = initialY;
        }
    }
}


void Player::update(float dt, int x, int y, World& w) {
    timeElapsed += dt; // Update auto attack cooldown

    if (powerup) { // Update the duration of the active powerup
        powerupTime -= dt; 
        if (powerupTime <= 0) { 
            powerup = false; 
            powerupTime = 0.f; 
        } 
    }

    if (powerupCDTim > 0.f) {  // Update the duration of the powerup timer
        powerupCDTim -= dt; 
        if (powerupCDTim < 0) 
            powerupCDTim = 0; 
    }

    if (aoeCDTim > 0.f) { // Update the cooldown of the AOE
        aoeCDTim -= dt; 
        if (aoeCDTim < 0) 
            aoeCDTim = 0; 
    }

    if (invul) { // Update the invulerability duration
        invulTime -= dt; 
        if (invulTime <= 0) { 
            invul = false; 
            invulTime = 0.f; 
        } 
    }

    float newX = x * dt * 60;
    float newY = y * dt * 60;

    playerMovement(newX, newY, w); // Move the player (specific playermovement function as the player has to not go on water + not leave the map bounds)

}

