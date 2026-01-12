#pragma once
#include "raylib.h"
#include "../combat/WeaponDefs.h"
#include "Enemy.h" 

enum class PlayerState { IDLE, RUN, JUMP, ATTACK };

class Player {
public:
    Player(); // 构造函数
    void Update(float dt, Enemy& enemy);
    void Draw();

private:
    Rectangle body;
    Vector2 velocity;
    bool facingRight;

    // 状态机
    PlayerState currentState;
    int stateTimer;
    
    // 战斗
    Weapon currentWeapon;
    int comboIndex;
    bool attackConnected;

    // 内部逻辑函数 (类似 static helper functions)
    void HandleMovement(float dt);
    void TransitionToAttack(int comboIdx);
    void ProcessAttackState(Enemy& enemy);
    Rectangle GetCurrentHitbox(const AttackMove& move);
};