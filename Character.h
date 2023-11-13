#ifndef CHARACTER_H
#define CHARACTER_H

#include "BaseCharacter.h"
#include "raylib.h"

class Character : public BaseCharacter
{
    public:
        Character(int windowWidth, int windowHeight);
        virtual void Tick(float deltaTime) override;
        virtual Vector2 GetScreenPos() override;
        Rectangle getWeaponRect() {return weaponCollisionRec;};
        float GetHealth() const { return health; };

        void TakeDamage(float damage, float deltaTime = 0.0f);
    private:
        Rectangle weaponCollisionRec{};
        int winWidth{};
        int winHeight{};
        Texture2D weapon = LoadTexture("characters/weapon_sword.png");
        float health{100.f};
        float invTimer = 3.0f;
};

#endif