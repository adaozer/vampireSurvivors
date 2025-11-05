#include "Player.h"
#include "EnemyManager.h"
#include "GamesEngineeringBase.h"
#include "Camera.h"
#include "Helper.h"
#include "World.h"

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    GamesEngineeringBase::Window canvas;
    canvas.create(1024, 768, "Vampire Survivors");
    bool running = true;

    Player p(canvas.getWidth() / 2, canvas.getHeight() / 2, "Resources/0.png", 100, 4, 15);
    GamesEngineeringBase::Timer tim;
    EnemyManager em;
    Camera cam(0, 0);

    TileSet tiles("Resources/");
    World w;
    
    while (running)
    {
        canvas.checkInput();

        canvas.clear();

        float dt = tim.dt();
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

        p.update(dt, x, y);
        cam.center(p.getX(), p.getY(), canvas);
        
        w.draw(canvas, cam, tiles);

        p.autoAttack(em.enemyarr, em.rangedarr);
        p.updateBullets(dt, canvas, cam);
        p.checkBulletEnemyCollision(em.enemyarr, em.rangedarr);
        em.update(canvas, dt, p, cam);
        p.draw(canvas, cam);

        if (!p.isAlive()) {
            std::cout << "GAME OVER!" << " Your score is " << score << std::endl;
            running = false;
        }

        canvas.present();
    }
    return 0;
}