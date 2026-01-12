#pragma once
#include "../entities/Player.h"
#include "../entities/Entity.h"
#include <memory>

class Game {
public:
    Game(int width, int height, const char* title);
    ~Game();

    void Run();

private:
    void Update();
    void Draw();
    void LoadResources();

    std::unique_ptr<Player> player;
    std::unique_ptr<Enemy> enemy;
};

