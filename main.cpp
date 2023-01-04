#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include <string>

// MAIN BEGINS
int main()
{
    // Window Size
    int windowDimemsion = 384;
    InitWindow(windowDimemsion, windowDimemsion, "CLASSY CLASH");

    // Background info
    Texture2D mapBG = LoadTexture("nature_tileset/WorldMap.png");
    Vector2 bgPos{0.0, 0.0};
    float aScale{4.0f}; // scale for map size

    // Instance of a character
    Character knight{windowDimemsion, windowDimemsion};

    // create props
    Prop props[2]{
        Prop{Vector2{600.f, 300.f}, LoadTexture("nature_tileset/Rock.png")},
        Prop{Vector2{400.f, 500.f}, LoadTexture("nature_tileset/Log.png")},
    };

    // create a goblin
    Enemy goblin(Vector2{0.f, 0.f}, LoadTexture("characters/goblin_idle_spritesheet.png"), LoadTexture("characters/goblin_run_spritesheet.png"));

    Enemy slime(Vector2{500.f, 500.f}, LoadTexture("characters/slime_idle_spritesheet.png"), LoadTexture("characters/slime_run_spritesheet.png"));
    Enemy slime1(Vector2{800.f, 800.f}, LoadTexture("characters/slime_idle_spritesheet.png"), LoadTexture("characters/slime_run_spritesheet.png"));
    Enemy slime2(Vector2{1000.f, 1000.f}, LoadTexture("characters/slime_idle_spritesheet.png"), LoadTexture("characters/slime_run_spritesheet.png"));

    

    Enemy *enemies[]{
        &goblin,
        &slime,
        &slime1,
        &slime2};

    for (auto enemy : enemies)
    {
        enemy->setTarget(&knight);
    }



    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        // Drawing begins
        //
        BeginDrawing();
        ClearBackground(WHITE);

        bgPos = Vector2Scale(knight.GetWorldPos(), -1.f);

        // Draw the background
        DrawTextureEx(mapBG, bgPos, 0.0, aScale, WHITE);

        // draw all props
        for (auto prop : props)
        {
            prop.Render(knight.GetWorldPos());
        }

        if (!knight.getAlive()) // character is dead
        {
            DrawText("Game Over!", 55.f, 45.f, 40, RED);
            EndDrawing();
            continue;
        }
        else // Character is alive
        {
            // OUTDATED HEALTH DISPLAY
                // std::string knightHealth = "Health: ";
                // knightHealth.append(std::to_string(knight.GetHealth()), 0, 5);
                // DrawText(knightHealth.c_str(), 55.f, 45.f, 40, RED);
        }

        // check map bounds
        knight.Tick(GetFrameTime());
        if (knight.GetWorldPos().x < 0.f ||
            knight.GetWorldPos().y < 0.f ||
            knight.GetWorldPos().x + windowDimemsion > mapBG.width * aScale ||
            knight.GetWorldPos().y + windowDimemsion > mapBG.height * aScale)
        {
            knight.undoMovement();
        }

        // check if we're colliding with props
        for (auto prop : props)
        {
            if (CheckCollisionRecs(prop.GetCollisionRec(knight.GetWorldPos()), knight.GetCollisionRec()))
            {
                knight.undoMovement();
            }
        }

        // move enemies
        for (int i = 0; i < sizeof(enemies) / sizeof(enemies[0]); i++)
        {
            enemies[i]->Tick(GetFrameTime());

        // PLEASE OPTIMIZE THIS WHOLE SECTION:(
            for (int j = 0; j < sizeof(enemies) / sizeof(enemies[0]); j++)
            {

                if (i == j) continue;
                // check if enemy collides into any other enemies
                if (i < sizeof(enemies) / sizeof(enemies[0]) - 1)
                {
                    if (CheckCollisionRecs(enemies[i]->GetCollisionRec(), enemies[j]->GetCollisionRec()))
                    {
                        enemies[i]->undoMovement();
                    }
                }
                // check for last enemy in array
                else
                {
                    if (CheckCollisionRecs(enemies[i]->GetCollisionRec(), enemies[j]->GetCollisionRec()))
                    {
                        enemies[i]->undoMovement();
                        // DrawText("POO", knight.GetScreenPos().x, knight.GetScreenPos().y, 22, BLUE);
                    }
                }
            }
        }

        // knight attack
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            for (auto enemy : enemies)
            {
                if (CheckCollisionRecs(enemy->GetCollisionRec(), knight.getWeaponRect()))
                {
                    enemy->setAlive(false);
                }
            }
        }

        // Health Bar!
        DrawRectangle(15, 15, 250.f, 60.f, GOLD);
        DrawRectangle(20, 20, 240, 50, BLACK);
        DrawRectangle(20, 20, (knight.GetHealth() * 240) / 100, 50, RED);
        DrawRectangle(20, 60, ((knight.GetHealth() * 240) / 100), 10, Color{200, 0, 0, 255});
        DrawRectangle(20, 20, ((knight.GetHealth() * 240) / 100), 5, Color{255, 100, 100, 255});
        DrawText(TextFormat("%.f/100", knight.GetHealth()), 82, 31, 35, GRAY);
        DrawText(TextFormat("%.f/100", knight.GetHealth()), 80, 30, 35, WHITE);

        EndDrawing();

        // GetPixelColor()
    }
    knight.unloadAllTexts();
    // goblin.unloadAllTexts();
    UnloadTexture(mapBG);
}