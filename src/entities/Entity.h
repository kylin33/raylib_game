#pragma once
#include "raylib.h"
#include <vector>

class Entity {
public:
    Rectangle body;
    Vector2 velocity;
    bool isGrounded; 

    Entity(float x, float y, float w, float h) {
        body = {x, y, w, h};
        velocity = {0, 0};
        isGrounded = false;
    }

    virtual ~Entity() = default;

    // 核心物理更新函数
    // dt: 帧时间
    // obstacles: 地图里的所有墙和地面
    void UpdatePhysics(float dt, const std::vector<Rectangle>& obstacles) {
        // 1. 应用重力
        velocity.y += 1000.0f * dt; // 假设重力是 1000

        // 2. 尝试垂直移动 (Y轴)
        body.y += velocity.y * dt;
        isGrounded = false; // 先假设在空中

        // 3. 垂直碰撞检测
        for (const auto& wall : obstacles) {
            if (CheckCollisionRecs(body, wall)) {
                // 如果当前是在下落 (velocity.y > 0) 并且撞到了墙的上方
                if (velocity.y > 0 && body.y < wall.y) {
                    body.y = wall.y - body.height; // 修正位置到墙顶
                    velocity.y = 0;
                    isGrounded = true;
                }
                // 如果是头顶撞到天花板 (可选)
                else if (velocity.y < 0 && body.y > wall.y + wall.height) {
                    body.y = wall.y + wall.height;
                    velocity.y = 0;
                }
            }
        }
        
        // 4. 尝试水平移动 (X轴)
        body.x += velocity.x * dt;

        // 5. 水平碰撞检测
        for (const auto& wall : obstacles) {
            if (CheckCollisionRecs(body, wall)) {
                // 简单的回弹或阻挡
                if (velocity.x > 0) body.x = wall.x - body.width; // 撞右边
                else if (velocity.x < 0) body.x = wall.x + wall.width; // 撞左边
                // velocity.x = 0; // 可选：撞墙停止
            }
        }
    }

    void DrawBody(Color color) {
        DrawRectangleRec(body, color);
    }
};