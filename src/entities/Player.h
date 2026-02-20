#pragma once
#include <vector>
#include <memory>
#include "raylib.h"
#include "../combat/WeaponDefs.h"
#include "Enemy.h" 

enum class PlayerState { IDLE, RUN, JUMP, ATTACK };

class Player {
public:
    Player(); // 构造函数
    void Update(float dt, std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<Rectangle>& mapObstacles);
    void Draw();
    Vector2 GetPosition() const { return { body.x + body.width / 2, body.y + body.height / 2 };}; 
    float GetHP() const { return hp; };
    float GetMaxHP() const { return maxHp; };
    void SetHP(float damage);

private:
    Rectangle body;
    Vector2 velocity;
    bool facingRight;
    float hp;
    float maxHp;

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
    void ProcessAttackState(std::vector<std::unique_ptr<Enemy>>& enemies);
    Rectangle GetCurrentHitbox(const AttackMove& move);
};