#include "Melee.h"

Melee::Melee(float _posX, float _posY, std::string filepath, int _health, int _speed, int _damage) : Character(_posX, _posY, filepath, _health, _speed, _damage) {}

void Melee::draw(GamesEngineeringBase::Window& canvas, Camera& cam) {
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

void Melee::move(Player& p, float dt) {
    float px = p.getX();
    float py = p.getY();

    float dx = px - posX;
    float dy = py - posY;
    float len2 = dx * dx + dy * dy;

    if (len2 > 0) {
        float inv = 1.0f / sqrtf(len2);
        float moveX = dx * inv * speed * dt * 60.0f;
        float moveY = dy * inv * speed * dt * 60.0f;

        updatePos(moveX, moveY);
    }
}