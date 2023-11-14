#ifndef ENEMY_H
#define ENEMY_H

#include "BaseCharacter.h"
#include "raylib.h"
#include "Character.h"

class Enemy : public BaseCharacter
{
    public:
        Enemy(Vector2 pos, Texture2D idle_texture, Texture2D run_texture, Character* heroTarget);
        virtual void Tick(float deltaTime) override;
        virtual Vector2 GetScreenPos() override;
        // keep track of the number of enemies remaining, static so that it can be shared across all enemy objects
        // static int enemyCount; // unnecessary because use of vector now?
        //void setTarget(Character* aGuy) { target = aGuy; }
        void ChaseState(float deltaTime);
        void DashState(float deltaTime, Vector2 targetPos);
        void StartDash();
    
    private:
        Character* target;
        float DPS{10.f};

        // distance from self to target
        float chaseRadius{125.f};

        // counts down until the enemy will dash once within range
        float counter = 2.f;

        Vector2 targetPos = {};

        // TEST
        // implementation of state machine
        enum class EnemyState {
            Chasing,
            Dashing
        };

        EnemyState currentState;
        float dashCounter;
        Vector2 lastKnownPosition;
};

#endif
