#include "EnemyManager.h"

int main(int argc, char* argv[]) {
    srand(static_cast<unsigned int>(time(nullptr)));

    GamesEngineeringBase::Window canvas;
    canvas.create(1024, 768, "Vampire Survivors");
    bool running = true;

    Player p(canvas.getWidth() / 2, canvas.getHeight() / 2, "Resources/0.png", 100, 4, 15);
    GamesEngineeringBase::Timer tim;
    EnemyManager em;
    Camera cam(0, 0);
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
        if (canvas.keyPressed('Z')) p.castAOE(em.enemyarr, em.currentSize, em.rangedarr, em.rangedCount, 5, cam, canvas);

        p.update(dt, x, y);
        cam.center(p.getX(), p.getY(), canvas);
        p.autoAttack(em.enemyarr, em.currentSize, em.rangedarr, em.rangedCount);
        p.updateBullets(dt, canvas, cam);
        p.checkBulletEnemyCollision(em.enemyarr, em.currentSize, em.rangedarr, em.rangedCount);
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