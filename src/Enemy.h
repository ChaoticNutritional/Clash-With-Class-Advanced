#ifndef ENEMY_H
#define ENEMY_H

#include "BaseCharacter.h"
#include "raylib.h"
#include "Character.h"

class Enemy : public BaseCharacter
{
    public:
        Enemy(Vector2 pos, Texture idle_texture, Texture2D run_texture);
        virtual void Tick(float deltaTime) override;
        virtual Vector2 GetScreenPos() override;
        // keep track of the number of enemies remaining 
        static int enemyCount;
        void setTarget(Character* aGuy) { target = aGuy; }


    
    private:
        Character* target;
        float DPS{10.f};
        float chaseRadius{20.f};
};

#endif
