#pragma once
#include "raylib.h"

class Enemy {
public:
    Rectangle body;
    bool isHit;
    int hitFlashTimer;

    Enemy(float x, float y); // 构造函数
    void Update();
    void Draw();
    void TakeDamage(float dmg);
};