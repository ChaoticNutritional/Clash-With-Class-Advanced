#ifndef BASE_CHARACTER_H
#define BASE_CHARACTER_H
#include "raylib.h"
#include "raymath.h"

class BaseCharacter
{
    public:
        BaseCharacter();
        Vector2 GetWorldPos() { return worldPos; }
        void SetActiveTexture(Texture2D);
        void undoMovement();
        Rectangle GetCollisionRec();
        virtual void Tick(float deltaTime);
        void unloadAllTexts();
        virtual Vector2 GetScreenPos() = 0;
        bool getAlive() {return alive;}
        void setAlive(bool youGood) {alive = youGood;}
        void SetColor(Color aColor);

    protected:
        Texture2D run{LoadTexture("characters/knight_run_spritesheet.png")};
        Texture2D idle{LoadTexture("characters/knight_idle_spritesheet.png")};
        Texture2D currentTexture{idle};
        Vector2 onScreenPos{};
        Vector2 worldPos{};
        Vector2 worldPosLastFrame{};
        int width{};
        int height{};
        float scale{4.0f};
        // facing value, -1 or 1
        float RightLeft{1.0f};
        bool damaged{false};
        Color hurtColor{RED};
        Color normalColor{WHITE};
        //Color attacking{255, 255, 255, 255}; 
        Color currentColor = normalColor;
        

        

        // Animation variables
        float runningTime{};
        int frame{};
        const int maxFrames{6};
        const float updateTime{1.f / 12.f};

        float speed{};

        Vector2 velocity{};

    private:
        bool alive{true};
        
};

#endif