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
        void setTarget(Character* aGuy) { target = aGuy;}
        virtual Vector2 GetScreenPos() override;
    
    private:
        Character* target;
        float DPS{10.f};
        float chaseRadius{100.f};
};

#endif
