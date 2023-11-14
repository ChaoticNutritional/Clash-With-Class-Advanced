#ifndef BASE_CHARACTER_H
#define BASE_CHARACTER_H
#include "raylib.h"
#include "raymath.h"

class BaseCharacter
{
    public:
        BaseCharacter();
        Vector2 GetWorldPos() { return worldPos; };
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

        // these three vars are in an attempt to adust the color of all pixels in a 2D texture, experimental, currently not in use
        Color hurtColor{RED};
        Color normalColor{WHITE};
        Color currentColor = normalColor;
        
        // Animation variables
        float runningTime{}; // maybe rename this variable for clarity after coming back to this program
        int frame{};
        const int maxFrames{6};
        float updateTime{1.f / 12.f};

        float speed{};

        Vector2 velocity{};
        bool alive{true};
        
};

#endif