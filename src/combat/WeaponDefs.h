#pragma once
#include "raylib.h"
#include <vector>
#include <string>

// 攻击动作帧数据
struct AttackMove {
    int startupFrames;
    int activeFrames;
    int recoveryFrames;
    
    Rectangle hitbox;
    float damage;
    
    int TotalDuration() const { return startupFrames + activeFrames + recoveryFrames; }
};

// 武器定义
struct Weapon {
    std::string name;
    std::vector<AttackMove> combos;
};