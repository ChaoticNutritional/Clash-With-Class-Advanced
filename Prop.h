#include "raylib.h"

class Prop
{
    public:
        // Constructor
        Prop(Vector2 pos, Texture2D tex);
        void Render(Vector2 knightPos);
        Rectangle GetCollisionRec(Vector2 knightPos);

    private:
        Vector2 worldPos{};
        Texture2D texture{};
        Vector2 onScreenPos{};
        float scale{4.f};
};