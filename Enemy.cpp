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
    // 
    DrawRectangleLines(GetScreenPos().x, GetScreenPos().y, 100, 100, VIOLET);

    // DEBUGGING
    // line from enemy position to target
    DrawLine(GetScreenPos().x, GetScreenPos().y, target->GetScreenPos().x, target->GetScreenPos().y, RED);

    // Box around target's world position
    DrawRectangleLines(Vector2Divide(target->GetWorldPos(), target->GetScreenPos()).x, Vector2Divide(target->GetWorldPos(), target->GetScreenPos()).y, 100, 100, GREEN);

    // Box around target position based in screen space
    DrawRectangleLines(targetPos.x, targetPos.y, 100, 100, RED);

    switch (currentState)
    {
        case EnemyState::Chasing:
            //DEBUGGING: std::cout << "Switch to chasing...." << std::endl;
            ChaseState(deltaTime);
            break;
             
        case EnemyState::Dashing:
            //DEBUGGING: std::cout << "Switch to dashing...." << std::endl;
            DashState(deltaTime);
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
    //DEBUGGING: std::cout << "Chasing...." << std::endl;

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

}

void Enemy::DashState(float deltaTime)
{
    targetPos = (Vector2Subtract());
    velocity = Vector2Subtract(this->targetPos, GetScreenPos());
    //DEBUGGING: std::cout << "distance left to dash: " << Vector2Length(velocity) << std::endl;

    // we've approximately reached our target
    if (Vector2Length(velocity) < 20.f)
    {
        //DEBUGGING: std::cout << "reached dash location...." << std::endl;
        this->targetPos = target->GetScreenPos();
        currentState = EnemyState::Chasing;
        speed = 3.f;
        //DEBUGGING: std::cout << "Going back to chase...." << std::endl;
        counter = 2.f;
        //updateTime *= 2.f;
    }

    // if enemy is standing on player
    if (CheckCollisionRecs(target->GetCollisionRec(), GetCollisionRec()))
    {
        if (!target->damaged)
        {
            target->TakeDamage(0);
        }
    }

    BaseCharacter::Tick(deltaTime);
}

void Enemy::StartDash()
{
    //DEBUGGING: std::cout << "Initiating dash...." << std::endl;

    // 
    Vector2* pos = new Vector2(target->GetScreenPos());

    std::cout << pos->x << " , " << pos->y << std::endl;

    currentState = EnemyState::Dashing;
    targetPos = Vector2Add(*pos, Vector2Scale(Vector2Subtract(*pos, GetScreenPos()), 1.5f));//Vector2Scale(target->GetScreenPos(), 1.5f);
    //updateTime *= .5f;
    speed = 6.f;

    // initiate switch to dash state
    // set current state enum to Dash?
        // handle speed change here?
        // speed += 10;
        // runningTime += <amount?>
}
