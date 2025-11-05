#include "Bullet.h"

Bullet::Bullet(float _posX, float _posY, float towardX, float towardY, bool _friendly, int _damage) : GameObject(_posX, _posY), friendly(_friendly), damage(_damage), alive(true) {
    if (friendly) {
        velocity = 200.f; // The friendly bullets go faster as they're supposed to be almost instant and accurate.
    }
    else {
        velocity = 100.f; // Enemy bullets go slower as the player needs to be able to dodge them
    }

    maxAge = 15.0f;
    age = 0;
    float dx = towardX - _posX;
    float dy = towardY - _posY;
    float len2 = dx * dx + dy * dy;

    radius = 4;

    if (len2 > 0) {
        float normalise = 1.0f / sqrtf(len2);
        vx = dx * normalise * velocity;
        vy = dy * normalise * velocity;
    }
    else {
        vx = velocity;
        vy = 0.f;
    }

}

bool Bullet::isOnScreen(Camera& cam, GamesEngineeringBase::Window& canvas) {
    float sx = posX - cam.getX();
    float sy = posY - cam.getY();
    return (sx >= -radius && sx <= canvas.getWidth() + radius && sy >= -radius && sy <= canvas.getHeight() + radius);
}

void Bullet::update(float dt, Player& p) {
    if (!alive) return;

    age += dt;
    if (age >= maxAge) { alive = false; return; }
    updatePos(vx * dt, vy * dt);

    if (!friendly) {
        int pcx = (int)p.getX() + p.image.width / 2;
        int pcy = (int)p.getY() + p.image.height / 2;
        int pr = p.image.width / 3;

        if (circlesIntersect((int)posX, (int)posY, radius, pcx, pcy, pr)) {
            p.takeDamage(damage);
            alive = false;
            return;
        }
    }
    else {

        if (posX < p.getX() - 512 || posX > p.getX() + 512 ||
            posY < p.getY() - 384 || posY > p.getY() + 384) {
            alive = false;

        }
    }
}


void Bullet::draw(GamesEngineeringBase::Window& canvas, Camera& cam) {
    if (!alive) return;
    if (!isOnScreen(cam, canvas)) return;

    int ix = (int)(posX - cam.getX());
    int iy = (int)(posY - cam.getY());

    for (int dx = -radius; dx <= radius; ++dx)
        for (int dy = -radius; dy <= radius; ++dy)
            if (dx * dx + dy * dy <= radius * radius) {
                int px = ix + dx;
                int py = iy + dy;
                if (px >= 0 && px < canvas.getWidth() && py >= 0 && py < canvas.getHeight()) {
                    if (friendly) {
                        canvas.draw(px, py, 255, 255, 255);
                    }
                    else {
                        canvas.draw(px, py, 255, 0, 0);
                    }
                }
            }
}