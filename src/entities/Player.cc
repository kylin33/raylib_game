#include "Player.h"

Player::Player() {
    body = { 200, 400, 32, 64 }; // 玩家碰撞箱
    velocity = { 0, 0 };
    facingRight = true;

    currentState  = PlayerState::IDLE;
    stateTimer = 0;        // 通用状态计时器 (帧计数)
    
    int comboIndex = 0;        // 当前连击段数
    bool attackConnected = false; // 是否在当前帧命中了敌人 (防止一帧多判)

    AttackMove slash1;
    slash1.startupFrames = 10;
    slash1.activeFrames = 6;
    slash1.recoveryFrames = 14;
    slash1.hitbox = { 20, -10, 60, 50 }; // 向前延伸的判定框
    slash1.damage = 10.0f;

    AttackMove slash2;
    slash2.startupFrames = 6;
    slash2.activeFrames = 4;
    slash2.recoveryFrames = 20;
    slash2.hitbox = { 20, 10, 80, 20 };
    slash2.damage = 15.0f;

    currentWeapon.name = "Great Sword";
    currentWeapon.combos.push_back(slash1);
    currentWeapon.combos.push_back(slash2);
}

void Player::Update(float dt, Enemy& enemy) {
    const float GRAVITY = 800.0f;
    velocity.y += GRAVITY * dt;

    // 状态机 Switch-Case (代码同之前)
    switch (currentState) {
        case PlayerState::IDLE:
        case PlayerState::RUN:
            HandleMovement(dt);
            if (IsKeyPressed(KEY_Z) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                TransitionToAttack(0);
            }
            break;
        case PlayerState::JUMP:
            HandleMovement(dt);
            if (IsKeyPressed(KEY_Z)) {
                // 这里可以添加跳劈逻辑
            }
            break;
        case PlayerState::ATTACK:
            ProcessAttackState(enemy);
            break;
    }
    
    // 物理积分
    if (currentState != PlayerState::ATTACK) {
        body.x += velocity.x * dt;
    }
    body.y += velocity.y * dt;
    
    // 地面碰撞
    if (body.y >= 400) {
        body.y = 400;
        velocity.y = 0;
        if (currentState == PlayerState::JUMP) currentState = PlayerState::IDLE;
    }
}

void Player::Draw() {
    // 绘制玩家
    DrawRectangleRec(body, BLUE);

    if (facingRight) DrawRectangle(body.x + 20, body.y + 10, 10, 10, SKYBLUE);
    else DrawRectangle(body.x - 0, body.y + 10, 10, 10, SKYBLUE);

    const char* stateStr = "Unknown";
    if (currentState == PlayerState::IDLE) stateStr = "IDLE";
    else if (currentState == PlayerState::RUN) stateStr = "RUN";
    else if (currentState == PlayerState::ATTACK) stateStr = "ATTACK";
    else if (currentState == PlayerState::JUMP) stateStr = "JUMP";
    DrawText(stateStr, body.x, body.y - 20, 10, GREEN);

    if (currentState == PlayerState::ATTACK) {
        const AttackMove& move = currentWeapon.combos[comboIndex];
        int frame = stateTimer;
        
        // 计算当前处于哪个阶段
        if (frame >= move.startupFrames && frame < (move.startupFrames + move.activeFrames)) {
            // ACTIVE 阶段：绘制红色攻击框
            Rectangle globalHitbox = GetCurrentHitbox(move);
            DrawRectangleLinesEx(globalHitbox, 2, RED); // 空心红框表示判定区
            // 半透明填充
            DrawRectangleRec(globalHitbox, { 255, 0, 0, 100 });
        } 
        else if (frame < move.startupFrames) {
            // STARTUP 阶段：黄色提示
            DrawCircle(body.x + 16, body.y - 10, 5, YELLOW);
        }
        else {
            // RECOVERY 阶段：蓝色提示
            DrawCircle(body.x + 16, body.y - 10, 5, BLUE);
        }
    }
}

void Player::HandleMovement(float dt) {
    const float SPEED = 300.0f;
    const float JUMP_FORCE = -450.0f;
    float moveInput = 0;
    if (IsKeyDown(KEY_RIGHT)) moveInput = 1;
    if (IsKeyDown(KEY_LEFT)) moveInput = -1;

    if (moveInput != 0) {
        velocity.x = moveInput * SPEED;
        facingRight = (moveInput > 0);
        if (currentState != PlayerState::JUMP) currentState = PlayerState::RUN;
    } else {
        velocity.x = 0;
        if (currentState != PlayerState::JUMP) currentState = PlayerState::IDLE;
    }

    // 跳跃
    if (IsKeyPressed(KEY_SPACE) && body.y >= 400) {
        velocity.y = JUMP_FORCE;
        currentState = PlayerState::JUMP;
    }
}

void Player::TransitionToAttack(int comboIdx){
    if (comboIdx >= currentWeapon.combos.size()) return; // 连击结束

    currentState = PlayerState::ATTACK;
    comboIndex = comboIdx;
    stateTimer = 0; // 重置帧计数器
    attackConnected = false; // 重置命中标记
    velocity.x = 0; // 攻击时定身 (Root Motion)
}

void Player::ProcessAttackState(Enemy& enemy) {
    stateTimer++; // 帧步进    
    const AttackMove& move = currentWeapon.combos[comboIndex];
    
    // 1. 判断是否处于 Active (判定) 阶段
    int activeStart = move.startupFrames;
    int activeEnd = activeStart + move.activeFrames;

    if (stateTimer >= activeStart && stateTimer < activeEnd) {
        // 只有这几帧才进行碰撞检测
        if (!attackConnected) { // 简单逻辑：每一段攻击只能命中一次
            Rectangle hitRect = GetCurrentHitbox(move);
            if (CheckCollisionRecs(hitRect, enemy.body)) {
                enemy.TakeDamage(move.damage);
                attackConnected = true; 
                // 这里可以加 HitStop (顿帧) 逻辑
            }
        }
    }

    // 2. 连击预输入窗口 (Input Buffer)
    // 在后摇阶段允许按键预存下一段攻击
    bool bufferAttack = false;
    if (stateTimer > activeEnd && (IsKeyPressed(KEY_Z) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) {
        bufferAttack = true;
    }

    // 3. 动作结束判定
    if (stateTimer >= move.TotalDuration()) {
        if (bufferAttack && (comboIndex + 1 < currentWeapon.combos.size())) {
            // 进入下一段连击
            TransitionToAttack(comboIndex + 1);
        } else {
            // 回归常态
            currentState = PlayerState::IDLE;
            comboIndex = 0;
        }
    }
}

Rectangle Player::GetCurrentHitbox(const AttackMove& move){
    float hitX = facingRight ? (body.x + move.hitbox.x) : (body.x + body.width - move.hitbox.x - move.hitbox.width);
    return { hitX, body.y + move.hitbox.y, move.hitbox.width, move.hitbox.height };
}