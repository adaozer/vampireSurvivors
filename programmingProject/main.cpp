#include "Player.h"
#include "EnemyManager.h"
#include "GamesEngineeringBase.h"
#include "Camera.h"
#include "Helper.h"
#include "World.h"


// Helper: parse CSV, player: player movement
// World, Tiles, Main


int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    GamesEngineeringBase::Window canvas;
    canvas.create(1024, 768, "Vampire Survivors");

    int mode;
    std::cout << "Pick gamemode \n 1: Infinite map\n 2: Finite map\n";
    std::cin >> mode;

    bool running = true;

    Player p(canvas.getWidth() / 2, canvas.getHeight() / 2, "Resources/characterSprite.png", 100, 7, 15);
    GamesEngineeringBase::Timer tim;
    EnemyManager em;
    Camera cam(0, 0);

    TileSet tiles;
    World w;
    w.loadUsingFile("Resources/tiles.txt");
    w.buildCollisionLayer();
    float fpsPrint = 0.f;

    while (running)
    {
        canvas.checkInput();

        canvas.clear();

        float dt = tim.dt();
        float fps = 1.0f / dt;
        fpsPrint += dt;
        int x = 0;
        int y = 0;

        if (canvas.keyPressed(VK_ESCAPE)) break;
        if (canvas.keyPressed('W')) y -= p.speed;
        if (canvas.keyPressed('S')) y += p.speed;
        if (canvas.keyPressed('A')) x -= p.speed;
        if (canvas.keyPressed('D')) x += p.speed;
        if (canvas.keyPressed('F')) p.activatePowerup();
        if (canvas.keyPressed('I')) {
            p.invul = true;
            p.invulTime = 1000000.f;
        }
        if (canvas.keyPressed('Z')) p.castAOE(em.enemyarr, em.rangedarr, 10, cam, canvas);

        p.update(dt, x, y, w);
        cam.center(p.getX(), p.getY(), canvas, w);
        w.draw(canvas, cam, tiles);
        p.autoAttack(em.enemyarr, em.rangedarr, cam, canvas);
        p.updateBullets(dt, canvas, cam);
        p.checkBulletEnemyCollision(em.enemyarr, em.rangedarr);
        em.update(canvas, dt, p, cam, w);
        p.draw(canvas, cam);

        if (fpsPrint > 1.0f) {
            std::cout << "FPS: " << fps << std::endl;
            fpsPrint = 0.f;
        }

        if (!p.isAlive()) {
            std::cout << "GAME OVER!" << " Your score is " << score << std::endl;
            running = false;
        }

        canvas.present();
    }
    return 0;
}