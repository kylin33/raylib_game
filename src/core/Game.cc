#include "Game.h"
#include "raylib.h"

Game::Game(int width, int height, const char* title) {
    InitWindow(width, height, title);
    SetTargetFPS(60);

    // init camera
    camera = { 0 };
    camera.target = {0, 0};
    camera.offset = { (float)width / 2.0f, (float)height / 2.0};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // 实例化对象
    player = std::make_unique<Player>();
    LoadLevel();
}

void Game::LoadLevel(){
    // Load map obstacles, enemies, etc.
    enemies.push_back(std::make_unique<Enemy>(600, 400));
    enemies.push_back(std::make_unique<Enemy>(800, 400));
    enemies.push_back(std::make_unique<Enemy>(1200, 400));

    mapObstacles.push_back({0, 464, 1200, 136});
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
    for(auto& enemy : enemies){
        enemy->Update();
    }
    player->Update(dt, enemies, mapObstacles);

    camera.target = player->GetPosition();
}

void Game::Draw(){
    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode2D(camera);
    for(const auto& wall: mapObstacles){
        DrawRectangleRec(wall, GRAY);
    }
    for(const auto& enemy: enemies){
        enemy->Draw();
    }
    player->Draw();

    DrawText("Controls: Arrow Keys to Move, SPACE to Jump, Z/Click to Attack", 10, 10, 20, DARKGRAY);
    EndMode2D();
    DrawText("HP: 100", 20, 20, 20, RED);

    EndDrawing();
}
