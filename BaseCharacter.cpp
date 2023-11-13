#include "BaseCharacter.h"
#include "raymath.h"
#include "raylib.h"

BaseCharacter::BaseCharacter(){

};

void BaseCharacter::undoMovement()
{
    worldPos = worldPosLastFrame; 
};

Rectangle BaseCharacter::GetCollisionRec()
{
    return Rectangle{
        GetScreenPos().x,
        GetScreenPos().y,
        width * scale,
        width * scale};
};

void BaseCharacter::unloadAllTexts()
{
    UnloadTexture(idle);
    UnloadTexture(run);
};

void BaseCharacter::Tick(float deltaTime)
{
    worldPosLastFrame = worldPos;

    // Update anim frame
    runningTime += deltaTime;
    if (runningTime >= updateTime)
    {
        frame++;
        runningTime = 0.f;
        if (frame > maxFrames)
            frame = 0;
    }

    // Draw Character
    // source: rectangle around sprite in the texture sheet
    Rectangle source{(float)width * frame, 0.f, RightLeft * (float)width, (float)height};
    // dest
    Rectangle dest{GetScreenPos().x, GetScreenPos().y, scale * (float)width, scale * height};

    // if direction is nonzero, move
    if (Vector2Length(velocity) != 0.0)
    {
        worldPos = Vector2Add(worldPos, Vector2Scale(Vector2Normalize(velocity), speed));

        velocity.x < 0.f ? RightLeft = -1.f : RightLeft = 1.f;

        currentTexture = run;
    }
    else
    {
        currentTexture = idle;
    }

    velocity = {};

    // Draw
    DrawTexturePro(currentTexture, source, dest, Vector2{}, 0.0f, currentColor);
};