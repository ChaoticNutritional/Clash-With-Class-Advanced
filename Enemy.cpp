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
    float counter = 7.f;
    bool dashing = false;
    target = heroTarget;
    targetPos = target->GetScreenPos();
}

void Enemy::Tick(float deltaTime)
{
    // DEBUGGING AND TESTING
    /*
    //velocity = Vector2Subtract(velocity, Vector2{ boxOfCollision.x, boxOfCollision.y });
    // DEBUGGING & notes
    //DrawLine(GetScreenPos().x, GetScreenPos().y, boxOfCollision.x, boxOfCollision.y, BLUE);

    // DEBUGGING FOR COLLISION OFFSET TESTING IN PROGRESS
    //DrawRectangleRec(boxOfCollision, BLUE);
   
    // line from enemy position to target
    //DrawLine(GetScreenPos().x, GetScreenPos().y, target->GetScreenPos().x, target->GetScreenPos().y, RED);

    // This might be fun to build a minimap in future development...
        // Box around target's world position
        //DrawRectangleLines(Vector2Divide(target->GetWorldPos(), target->GetScreenPos()).x, Vector2Divide(target->GetWorldPos(), target->GetScreenPos()).y, 100, 100, GREEN);

    // Box around target position based in screen space
    //DrawRectangleLines(targetPos.x, targetPos.y, 100, 100, RED);
    */
    velocity = Vector2Add(velocity, Vector2Normalize(Vector2{ boxOfCollision.x, boxOfCollision.y }));
    
    // check for collisions between any other enemies...
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
    velocity = Vector2Add(velocity, Vector2Normalize(Vector2{boxOfCollision.x, boxOfCollision.y }));

    // if enemy collides with player
    if (CheckCollisionRecs(target->GetCollisionRec(), GetCollisionRec()))
    {
        if (!target->damaged)
        {
            target->TakeDamage(5);
        }
    }

    // if the length of the direction vector from self to player is within the player radius, do <something>
        // todo: Dash attack
    if (Vector2Length(velocity) < chaseRadius)
    {
        // stop moving
        velocity = {};

        if (flashed)
        {
            currentColor = LIGHTGRAY;
        }
        else
        {
            currentColor = YELLOW;
        }

        flashCounter -= deltaTime;

        if (flashCounter < 0.f)
        {
            flashed = !flashed;
            flashCounter = 0.2f;
        }

        // wait time until dash
        Enemy::counter -= deltaTime;

        // Consider a charging state?
        // initiates transition to dash state
        if (counter <= 0.f)
        {
            StartDash();
        }
    }
    else { currentColor = WHITE; }

    BaseCharacter::Tick(deltaTime);

}

void Enemy::DashState(float deltaTime)
{
    targetPos = Vector2Subtract(targetPos, Vector2Scale(Vector2Normalize(target->getVelocity()), 5.0f));
    velocity = Vector2Subtract(this->targetPos, GetScreenPos());
    velocity = Vector2Add(velocity, Vector2Normalize(Vector2{ boxOfCollision.x, boxOfCollision.y }));
    //DEBUGGING: std::cout << "distance left to dash: " << Vector2Length(velocity) << std::endl;

    // we've approximately reached our target
    if (Vector2Length(velocity) < 20.f)
    {
        //DEBUGGING: std::cout << "reached dash location...." << std::endl;
        this->targetPos = target->GetScreenPos();
        currentState = EnemyState::Chasing;
        speed = 3.f;
        //DEBUGGING: std::cout << "Going back to chase...." << std::endl;
        counter = 1.f;
        updateTime = 1.f / 12.f;
    }

    // if enemy is standing on player
    if (CheckCollisionRecs(target->GetCollisionRec(), GetCollisionRec()))
    {
        if (!target->damaged)
        {
            target->TakeDamage(15);
        }
    }

    BaseCharacter::Tick(deltaTime);
}

void Enemy::StartDash()
{
    //DEBUGGING: std::cout << "Initiating dash...." << std::endl;

    // 
    Vector2 pos = Vector2(target->GetScreenPos());

    //std::cout << pos.x << " , " << pos.y << std::endl;

    currentState = EnemyState::Dashing;

    // an extension of 
    targetPos = Vector2Add(pos, Vector2Scale(Vector2Subtract(pos, GetScreenPos()), 1.5f));//Vector2Scale(target->GetScreenPos(), 1.5f);
    updateTime *= .5f;
    speed = 8.f;
}
