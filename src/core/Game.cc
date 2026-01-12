#include "Game.h"
#include "raylib.h"

Game::Game(int width, int height, const char* title) {
    InitWindow(width, height, title);
    SetTargetFPS(60);

    // 实例化对象
    player = std::make_unique<Player>();
    enemy = std::make_unique<Enemy>(600, 400); 
}


Game::~Game(){
    CloseWindow();
}

void Game::Run(){
    while (!WindowShouldClose()){
        Update();
        Draw();
    }
}

void Game::Update(){
    float dt = GetFrameTime();
    enemy->Update();
    player->Update(dt, *enemy);
}

void Game::Draw(){
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawRectangle(0, 464, 800, 136, LIGHTGRAY);
    DrawText("Controls: Arrow Keys to Move, SPACE to Jump, Z/Click to Attack", 10, 10, 20, DARKGRAY);

    enemy->Draw();
    player->Draw();
    EndDrawing();
}
