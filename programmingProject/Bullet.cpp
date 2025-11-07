#include "Bullet.h"
#include "Player.h"

Bullet::Bullet(float _posX, float _posY, float towardX, float towardY, bool _friendly, int _damage) : GameObject(_posX, _posY), friendly(_friendly), damage(_damage), alive(true) {
    if (friendly) {
        velocity = 250.f; // The friendly bullets go faster as they're supposed to be almost instant and accurate.
    }
    else {
        velocity = 150.f; // Enemy bullets go slower as the player needs to be able to dodge them
    }

    maxAge = 12.0f; // For enemy bullets mostly. Going to be used to destroy bullets after 12 seconds if they havent hit anything
    age = 0;
    float dx = towardX - _posX;
    float dy = towardY - _posY;
    float len2 = dx * dx + dy * dy; // Calculate the distance between the bullet's starting position and the target

    radius = 4;
    
    if (len2 > 0) { // to stop division by 0
        float normalise = 1.0f / sqrtf(len2);
        vx = dx * normalise * velocity;
        vy = dy * normalise * velocity; // Direction vector to move the bullet in a specific direction
    }
    else {
        vx = velocity;
        vy = 0.f;
    }

}

void Bullet::update(float dt, Player& p, GamesEngineeringBase::Window &canvas, Camera& cam) {
    if (!alive) return;

    age += dt; // Increase the age of the bullet and make sure its not on the screen for too long
    if (age >= maxAge) {
        alive = false; 
        return; 
    }
    updatePos(vx * dt, vy * dt); // move the bullet

    if (!friendly) { // If its an enemy bullet
        int pcx = p.getX() + p.image.width / 2;
        int pcy = p.getY() + p.image.height / 2;
        int pr = p.image.width / 3;
        // Check for collision
        if (circlesIntersect(posX, posY, radius, pcx, pcy, pr)) {
            p.takeDamage(damage);
            alive = false;
            return;
        }
    }
    else { // Only if its the player's bullet
        if (!onScreen(posX - radius, posY - radius, radius * 2, radius * 2, cam, canvas)) { // If the bullet isn't on screen, destroy it
            alive = false;
        }
    }
}

void Bullet::draw(GamesEngineeringBase::Window& canvas, Camera& cam) { 
    // Draw the bullet, standard function
    if (!alive || !onScreen(posX - radius, posY - radius, radius * 2, radius * 2, cam, canvas)) return;

    // Don't draw if its not on screen

    int ix = (int)(posX - cam.getX());
    int iy = (int)(posY - cam.getY());

    // Draw little circles based on the radius

    for (int dx = -radius; dx <= radius; ++dx)
        for (int dy = -radius; dy <= radius; ++dy)
            if (dx * dx + dy * dy <= radius * radius) {
                int px = ix + dx;
                int py = iy + dy;
                if (px >= 0 && px < canvas.getWidth() && py >= 0 && py < canvas.getHeight()) {
                    // White if friendly, red if enemy
                    if (friendly) {
                        canvas.draw(px, py, 255, 255, 255);
                    }
                    else {
                        canvas.draw(px, py, 255, 0, 0);
                    }
                }
            }
}