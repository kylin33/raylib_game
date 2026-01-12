#include "Enemy.h"
#include <iostream>

Enemy::Enemy(float x, float y){
    body = {x, y, 40, 60};
    isHit = false;
    hitFlashTimer = 0;
}

void Enemy::Update(){
    if(isHit){
        hitFlashTimer --;
        if(hitFlashTimer <= 0){
            isHit = false;
        }
    }
}

void Enemy::Draw(){
    DrawRectangleRec(body, isHit ? RED : DARKGRAY);
    DrawText("Enemy", body.x, body.y - 20, 10, GRAY);
}

void Enemy::TakeDamage(float dmg){
    isHit = true;
    hitFlashTimer = 10;
    std::cout << "Enemy took " << dmg << " damage!" << std::endl;
}