#include "Ranged.h"
#include "Player.h"
#include "Bullet.h"

Ranged::Ranged(float _posX, float _posY, std::string filepath, int _health, int _speed, int _damage) : Character(_posX, _posY, filepath, _health, _speed, _damage) {
    speed = 0;
    for (unsigned int i = 0; i < bulletSize; i++)
        enemyBarr[i] = nullptr;

}

void Ranged::draw(GamesEngineeringBase::Window& canvas, Camera& cam) {
    int ix = (int)(posX - cam.getX());
    int iy = (int)(posY - cam.getY());
    for (unsigned int k = 0; k < image.height; k++)
        if (k + iy > 0 && k + iy < canvas.getHeight()) {
            for (unsigned int j = 0; j < image.width; j++)
                if (j + ix > 0 && j + ix < canvas.getWidth())
                    if (image.alphaAtUnchecked(j, k) > 0)
                        canvas.draw(ix + j, iy + k, image.atUnchecked(j, k));
        }
}

void Ranged::shoot(Player& p) {
    // Creates a bullet that shoots toward the player. Basically just makes an object at the first null space found.
    float px = p.getX();
    float py = p.getY();
    for (int i = 0; i < bulletSize; ++i) {
        if (enemyBarr[i] == nullptr) {
            enemyBarr[i] = new Bullet(posX + image.width / 2, posY + image.height / 2, px + p.image.width / 2, py + p.image.height / 2, false, damage);
            return;
        }
    }
}

void Ranged::update(float dt, Player& p, GamesEngineeringBase::Window& canvas, Camera& cam) {
    if (!alive) return;
    // Shoots every x seconds
    shootTimer -= dt;
    if (shootTimer <= 0.0f) {
        shoot(p);
        shootTimer = shootingSpeed;
    }

    // Updates its bullets in the array
    for (int i = 0; i < bulletSize; ++i) {
        Bullet* b = enemyBarr[i];
        if (!b) continue;
        b->update(dt, p, canvas, cam);
        if (!b->alive) { // Free up space if the bullet isn't live anymore
            delete b;
            enemyBarr[i] = nullptr;
        }
        else {
            if (onScreen(b->getX() - b->radius, b->getY() - b->radius, b->radius * 2, b->radius * 2, cam, canvas)) {
                b->draw(canvas, cam); // Draw the bullets if they're on screen
            }
        }
    }
}