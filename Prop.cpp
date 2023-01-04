#include "Prop.h"
#include "raymath.h"

Prop::Prop(Vector2 pos, Texture2D tex): 
    worldPos(pos),
    texture(tex)
{

}

void Prop::Render(Vector2 knightPos)
{
    onScreenPos = Vector2Subtract(worldPos, knightPos);
    DrawTextureEx(texture, onScreenPos, 0.0f, scale, WHITE);
}

Rectangle Prop::GetCollisionRec(Vector2 knightPos)
{
    Vector2 onScreenPos{ Vector2Subtract(worldPos, knightPos)};
    return Rectangle{
        onScreenPos.x,
        onScreenPos.y,
        texture.width * scale,
        texture.height * scale
    };
}