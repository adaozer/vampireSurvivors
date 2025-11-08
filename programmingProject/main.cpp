#include "Player.h"
#include "EnemyManager.h"
#include "GamesEngineeringBase.h"
#include "Camera.h"
#include "Helper.h"
#include "World.h"
// draw, draw infinite, iswalkableinfinite, tileatinfinite, BUGLAR: Infinite dünyada bazi sularin collisioni tuhaf + player bi yerden sonra auto atmayi kesiyo
int main() {
    srand(static_cast<unsigned int>(time(nullptr))); // So random spawning works properly

    GamesEngineeringBase::Window canvas;
    canvas.create(1024, 768, "Vampire Survivors"); // Create the window

    std::cout << "Pick gamemode \n 1: Finite map\n 2: Infinite map\n"; // Mode selection through the console
    std::cin >> mode;

    bool running = true;

    Player p(canvas.getWidth() / 2, canvas.getHeight() / 2, "Resources/characterSprite.png", 100, 7, 15); // Create the player object
    GamesEngineeringBase::Timer tim; // Create the timer
    EnemyManager em; // Create the enemy manager
    Camera cam(0, 0); // Create the camera at 0,0 so we start at the centre

    TileSet tiles; // Load the map and the collision layer so we can't walk on water
    World w;
    w.loadUsingFile("Resources/tiles.txt");
    w.collisionLayer();
    float fpsPrint = 0.f;

    while (running)
    {
        canvas.checkInput();

        canvas.clear();

        float dt = tim.dt(); // Calculate dt, also the fps
        float fps = 1.0f / dt;
        fpsPrint += dt;
        int x = 0;
        int y = 0;

        if (canvas.keyPressed(VK_ESCAPE)) break;
        if (canvas.keyPressed('W')) y -= p.speed;
        if (canvas.keyPressed('S')) y += p.speed;
        if (canvas.keyPressed('A')) x -= p.speed;
        if (canvas.keyPressed('D')) x += p.speed;
        if (canvas.keyPressed('F')) p.activatePowerup(); // For player movement, powerup, AOE and invulnerability hack
        if (canvas.keyPressed('I')) {
            p.invul = true;
            p.invulTime = 1000000.f;
        }
        if (canvas.keyPressed('Z')) p.castAOE(em.enemyarr, em.rangedarr, 10, cam, canvas);

        p.update(dt, x, y, w); // All the update functions. Update player position and cooldowns here
        cam.center(p.getX(), p.getY(), canvas, w); // Center the camera
        if (mode == 1) w.draw(canvas, cam, tiles);  // Draw tiles as the camera moves
        if (mode == 2) w.drawInfinite(canvas, cam, tiles); // Different draw function if infinite world is selected
        p.autoAttack(em.enemyarr, em.rangedarr, cam, canvas); // Player auto attack
        p.updateBullets(dt, canvas, cam); // Move the bullets
        p.checkBulletEnemyCollision(em.enemyarr, em.rangedarr); // Check if the bullets hit a target
        em.update(canvas, dt, p, cam, w); // Update the enemies
        p.draw(canvas, cam); // Draw the player
        
        if (fpsPrint > 1.0f) { // Print fps
            std::cout << "FPS: " << fps << std::endl;
            fpsPrint = 0.f;
        }

        if (!p.isAlive()) { // If the player is dead kill the game
            std::cout << "GAME OVER!" << " Your score is " << score << std::endl;
            running = false;
        }

        canvas.present(); 
    }
    return 0;
}