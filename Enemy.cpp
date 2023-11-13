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
    float counter = 2.f;
    bool dashing = false; 
}

void Enemy::Tick(float deltaTime)
{

    // not dashing:
    // 
    // distance to target
    velocity = Vector2Subtract(target->GetScreenPos(), GetScreenPos());

    // if the length of the direction vector from self to player is within the player radius, do <something>
        // todo: Dash attack
    if(Vector2Length(velocity) < chaseRadius)
    {
        // stop moving
        velocity = {};

        // wait time until dash
        Enemy::counter -= deltaTime;

        // if counter <= 0.f
        // move quickly towards the position of the target, with some added distance
            // Vector2Scale(target->GetScreenPos(), <some amount>)

        // after dash finishes, reset the counter and go back to chasing.

        // maybe implement a statemachine? dashing state?
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