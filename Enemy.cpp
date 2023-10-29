#include "Enemy.h"
#include "raymath.h"

int Enemy::enemyCount;

Enemy::Enemy(Vector2 pos, Texture2D idle_texture, Texture2D run_texture)
{
    worldPos = pos;
    idle = idle_texture;
    run = run_texture;
    currentTexture = idle;
    width = currentTexture.width / maxFrames;
    height = currentTexture.height;
    speed = 3.f;
    Enemy::enemyCount++;
}

void Enemy::Tick(float deltaTime)
{
    // get toTarget/
    velocity = Vector2Subtract(target->GetScreenPos(), GetScreenPos());

    // if enemy is within range, do a chase, otherwise don't
    if(Vector2Length(velocity) < chaseRadius)
    {
        velocity = {};
        // IMPLEMENTATION FOR LATER: Change Color: Attack lunge
    }

    BaseCharacter::Tick(deltaTime);
    
    // if enemy is standing on player
    if(CheckCollisionRecs(target->GetCollisionRec(), GetCollisionRec()))
    {
        if(!target->damaged)
        {
            target->TakeDamage(DPS * deltaTime, GetFrameTime());
        }
    }
}

Vector2 Enemy::GetScreenPos()
{
    return Vector2Subtract(worldPos, target->GetWorldPos());
}