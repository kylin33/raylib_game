#include "Enemy.h"
#include "raymath.h"
#include <iostream>

Enemy::Enemy(float x, float y){
    body = {x, y, 40, 60};
    active = true;

    maxHp = 100;
    hp = maxHp;
    speed = 1.0f;
    damagePower = 10.0f;

    detectionRange = 300.0f;
    attackRange = 50.0f;

    currentState = EnemyState::IDLE;

    isHit = false;
    hitFlashTimer = 0;

    isAttacking = false;
    attackCooldown = 60.0f;
    attackTimer = 0;
}

void Enemy::Update(Vector2 playerPos){
    if(!active ) return ;
    if(isHit){
        hitFlashTimer --;
        if(hitFlashTimer <= 0) isHit = false;
    }
    if(attackTimer > 0){
        attackTimer --;
        isAttacking = false;
    }

    Vector2 enemyCenter = {body.x + body.width/2, body.y + body.height/2};
    float distToPlayer = Vector2Distance(enemyCenter, playerPos);
    switch (currentState) {
        case EnemyState::IDLE:
            if (distToPlayer < detectionRange) {
                currentState = EnemyState::CHASE;
            }
            break;

        case EnemyState::CHASE:
            // 逻辑：如果玩家太远，放弃追逐
            if (distToPlayer > detectionRange * 1.5f) { 
                currentState = EnemyState::IDLE;
            }
            // 逻辑：如果进入攻击范围，切换到攻击
            else if (distToPlayer <= attackRange) {
                currentState = EnemyState::ATTACK;
            }
            else {
                // 追逐玩家：简单的向量归一化移动 ?? 优化包括类似玩家的操作跳跃，移动，闪避等
                Vector2 direction = Vector2Subtract(playerPos, enemyCenter);
                direction = Vector2Normalize(direction);
                
                body.x += direction.x * speed;
                body.y += direction.y * speed;
            }
            break;

        case EnemyState::ATTACK:
            if (distToPlayer > attackRange) {
                currentState = EnemyState::CHASE;
            }
            else {
                // 执行攻击
                if (attackTimer <= 0) {
                    isAttacking = true; // 这一帧产生伤害判定
                    attackTimer = attackCooldown; // 重置冷却
                    std::cout << "Enemy Attacked!" << std::endl;
                }
            }
            break;
    }
}

void Enemy::Draw() {
    if (!active) return;

    // 根据状态决定颜色，方便调试
    Color color = DARKGRAY;
    
    if (isHit) color = RED; // 受伤变红
    else if (currentState == EnemyState::CHASE) color = ORANGE; // 追击变橙
    else if (currentState == EnemyState::ATTACK) color = PURPLE; // 攻击变紫
    
    DrawRectangleRec(body, color);

    // 绘制血条 (位于敌人头顶)
    float hpPercent = hp / maxHp;
    DrawRectangle(body.x, body.y - 10, body.width, 5, BLACK); // 血条背景
    DrawRectangle(body.x, body.y - 10, body.width * hpPercent, 5, GREEN); // 当前血量

    // 调试信息
    if (currentState == EnemyState::ATTACK && attackTimer > 0) {
        DrawText("Cooldown...", body.x, body.y - 30, 10, YELLOW);
    }
}

void Enemy::TakeDamage(float dmg) {
    if (!active) return;
    
    hp -= dmg;
    isHit = true;
    hitFlashTimer = 10;
    
    std::cout << "Enemy HP: " << hp << std::endl;

    if (hp <= 0) {
        active = false; // 敌人死亡
        std::cout << "Enemy Died!" << std::endl;
    }
}