#pragma once
#include "../entities/Player.h"
#include "../entities/Entity.h"
#include <vector>
#include <memory>
#include "raylib.h"

class Game {
public:
    Game(int width, int height, const char* title);
    ~Game();

    void Run();

private:
    void Update();
    void Draw();
    void LoadLevel(const std::string& filename);
    void DrawMinimap();

    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Enemy>> enemies;

    std::vector<Rectangle> mapObstacles;

    Camera2D camera;
};

