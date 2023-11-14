#include "Enemy.h"
#include "raymath.h"
#include <iostream>



Enemy::Enemy(Vector2 pos, Texture2D idle_texture, Texture2D run_texture, Character* heroTarget)
{
    worldPos = pos;
    idle = idle_texture;
    run = run_texture;
    currentTexture = idle;
    width = currentTexture.width / maxFrames;
    height = currentTexture.height;
    speed = 3.f;
    //enemyCount++; using vector size to keep track of enemies now...
    float counter = 2.f;
    bool dashing = false;
    target = heroTarget;
    targetPos = target->GetScreenPos();
}

void Enemy::Tick(float deltaTime)
{
    DrawLine(GetScreenPos().x, GetScreenPos().y, target->GetScreenPos().x, target->GetScreenPos().y, RED);
    DrawRectangleLines(targetPos.x, targetPos.y, 100, 100, RED);

    switch (currentState)
    {
        case EnemyState::Chasing:
            std::cout << "Switch to chasing...." << std::endl;
            ChaseState(deltaTime);
            break;
             
        case EnemyState::Dashing:
            std::cout << "Switch to dashing...." << std::endl;
            DashState(deltaTime, targetPos);
            break;

        default:
            ChaseState(deltaTime);
    }


}

Vector2 Enemy::GetScreenPos()
{
    return Vector2Subtract(worldPos, target->GetWorldPos());
}

void Enemy::ChaseState(float deltaTime)
{
    std::cout << "Chasing...." << std::endl;
    // new base state, put everything from Enemy::Tick in here now

    // distance to target
    velocity = Vector2Subtract(target->GetScreenPos(), GetScreenPos());

    // if the length of the direction vector from self to player is within the player radius, do <something>
        // todo: Dash attack
    if (Vector2Length(velocity) < chaseRadius)
    {
        // stop moving
        velocity = {};

        // wait time until dash
        Enemy::counter -= deltaTime;

        // Consider a charging state?
        // initiates transition to dash state
        if (counter <= 0.f)
        {
            StartDash();
        }
    }

    BaseCharacter::Tick(deltaTime);


    // TODO: move this out of this function!!!
    // if enemy is standing on player
    if (CheckCollisionRecs(target->GetCollisionRec(), GetCollisionRec()))
    {
        if (!target->damaged)
        {
            target->TakeDamage(DPS * deltaTime, GetFrameTime());
        }
    }
}

void Enemy::DashState(float deltaTime, Vector2 targetPos)
{
    velocity = Vector2Subtract(this->targetPos, GetScreenPos());
    std::cout << "distance left to dash: " << Vector2Length(velocity) << std::endl;

       
        // would like to do operator overload... investigate this soon tbd
    if (Vector2Length(velocity) < 20.f)
    {
        std::cout << "reached dash location...." << std::endl;
        this->targetPos = target->GetScreenPos();
        currentState = EnemyState::Chasing;
        speed = 3.f;
        std::cout << "Going back to chase...." << std::endl;
        counter = 2.f;
    }

    BaseCharacter::Tick(deltaTime);
}

void Enemy::StartDash()
{
    std::cout << "Initiating dash...." << std::endl;
    currentState = EnemyState::Dashing;
    targetPos = Vector2Add(target->GetScreenPos(), Vector2Scale(Vector2Subtract(target->GetScreenPos(), GetScreenPos()), 1.5f));//Vector2Scale(target->GetScreenPos(), 1.5f);
    //updateTime *= 2.f;
    speed = 6.f;

    // initiate switch to dash state
    // set current state enum to Dash?
        // handle speed change here?
        // speed += 10;
        // runningTime += <amount?>
}
