#include "Game.h"
#include "raylib.h"
#include <iostream>
#include <algorithm>

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
        enemy->Update(player->GetPosition());
        if (enemy->active && enemy->IsAttacking()) {
            player->SetHP(enemy->GetDamage());
        }
    }
    std::erase_if(enemies, 
        [](const auto& e) { return !e->active; });
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

    DrawText("Use Arrow Keys to Move", 0, 400, 20, LIGHTGRAY);
    EndMode2D();
    float hp = player->GetHP();
    float maxHp = player->GetMaxHP();
    float hpPercent = hp / maxHp; // 计算百分比 (0.0 到 1.0)
    
    // 防止百分比小于0 (如果玩家hp变负数)
    if (hpPercent < 0) hpPercent = 0;

    // 2. 定义 UI 位置和尺寸
    float barX = 20;
    float barY = 20;
    float barWidth = 200;
    float barHeight = 20;

    // 3. 绘制血条背景 (深红色或灰色)
    DrawRectangle(barX, barY, barWidth, barHeight, Fade(RED, 0.3f)); // 半透明红
    DrawRectangleLines(barX, barY, barWidth, barHeight, DARKGRAY);   // 边框

    // 4. 绘制当前血量 (鲜红色)
    DrawRectangle(barX, barY, barWidth * hpPercent, barHeight, RED);

    // 5. 绘制文字数值 (例如 "HP: 80 / 100")
    // 使用 TextFormat 格式化字符串
    const char* hpText = TextFormat("HP: %.0f / %.0f", hp, maxHp);
    DrawText(hpText, barX + 10, barY + 2, 20, WHITE);
    DrawText("Controls: Z to Attack", 20, 50, 20, DARKGRAY);
    DrawMinimap();

    EndDrawing();
}

void Game::DrawMinimap(){
    int mapSize = 150;
    int margin = 20;
    int screenW = GetScreenWidth();

    int mapX = screenW - mapSize - margin;
    int mapY = margin;

    Camera2D miniCam = {0};
    miniCam.target = player->GetPosition();

    miniCam.offset = {(float)mapX+mapSize/2, (float)mapY+mapSize/2};
    miniCam.rotation = 0.0f;
    miniCam.zoom = 0.2f;

    DrawRectangle(mapX, mapY, mapSize, mapSize, Fade(BLACK, 0.7f));

    BeginScissorMode(mapX, mapY, mapSize, mapSize);

    BeginMode2D(miniCam);
        for(const auto& wall: mapObstacles){
            DrawRectangleRec(wall, LIGHTGRAY);
        }

        // B. 绘制敌人 (用醒目的红点表示)
        for(const auto& enemy: enemies){
            // 假设 enemy 有一个 GetPosition 或直接访问 body
            // 这里为了简单演示直接画圆，你可以根据 enemy.body 来画
            Rectangle body = enemy->body; // 假设这是 public 的
            DrawRectangleRec(body, RED); 
        }

        // C. 绘制玩家 (用绿色表示自己)
        Vector2 pPos = player->GetPosition();
        DrawCircle(pPos.x, pPos.y, 20, GREEN); // 画个圆代表玩家
        
    EndMode2D();
    EndScissorMode();
    DrawRectangleLines(mapX, mapY, mapSize, mapSize, WHITE);
    DrawText("MAP", mapX+5, mapY+5, 10, WHITE);
}