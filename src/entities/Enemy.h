#pragma once
#include "raylib.h"

enum class EnemyState {
    IDLE,
    CHASE,
    ATTACK
};

class Enemy {
public:
    Rectangle body;
    bool active;

    Enemy(float x, float y); // 构造函数
    void Update(Vector2 playerPos); // 新增接收玩家位置
    void Draw();
    void TakeDamage(float dmg);

    float GetDamage() const { return damagePower;};
    bool IsAttacking() const { return isAttacking;};
private:
    float hp;
    float maxHp;
    float speed;
    float damagePower;

    float detectionRange;
    float attackRange;

    EnemyState currentState;
    bool isHit;
    int hitFlashTimer;

    bool isAttacking;
    float attackCooldown;
    float attackTimer;
};