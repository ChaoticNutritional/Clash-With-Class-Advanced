#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>


// fun but inefficient... read documentation of RLAPI for better alternative
void DrawPlayerHealth(Character knight) {
    DrawRectangle(15, 15, 250.f, 60.f, GOLD);
    DrawRectangle(20, 20, 240, 50, BLACK);
    DrawRectangle(20, 20, (knight.GetHealth() * 240) / 100, 50, RED);
    DrawRectangle(20, 60, ((knight.GetHealth() * 240) / 100), 10, Color{ 200, 0, 0, 255 });
    DrawRectangle(20, 20, ((knight.GetHealth() * 240) / 100), 5, Color{ 255, 100, 100, 255 });
    DrawText(TextFormat("%.f/100", knight.GetHealth()), 82, 31, 35, GRAY);
    DrawText(TextFormat("%.f/100", knight.GetHealth()), 80, 30, 35, WHITE);
}

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

    // enemy textures paired with running, would be better to use different data structure? for now testing this solution
    std::vector<Texture2D> enemyTextures
    { 
        LoadTexture("characters/goblin_idle_spritesheet.png"), LoadTexture("characters/goblin_run_spritesheet.png"),
        LoadTexture("characters/slime_idle_spritesheet.png") , LoadTexture("characters/slime_run_spritesheet.png"),
    };


    //TODO
        // location generator
        // Initializiation of enemies can occur while game runs, to add new guys after game start
        // enemy other collision checker
    
    

    // I don't want to have to specify one and the one after... ideate on solution for this
    Enemy goblin(Vector2{0.f, 0.f}, enemyTextures.at(0), enemyTextures.at(1), &knight);

    // todo: find way to define a slime enemy will always use those two textures as idle and run
        //MULTI ENEMY SETUP
    Enemy slime(Vector2{500.f, 500.f}, enemyTextures.at(2), enemyTextures.at(3), &knight);
    Enemy slime1(Vector2{800.f, 800.f}, enemyTextures.at(2), enemyTextures.at(3), &knight);
    Enemy slime2(Vector2{1000.f, 1000.f}, enemyTextures.at(2), enemyTextures.at(3), &knight);

    // using vector here to add enemies later
    std::vector<Enemy*> enemies = {
        &goblin,
        &slime, 
        &slime1,
        &slime2
    };

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        // Drawing begins
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

        // check map bounds
        knight.Tick(GetFrameTime());
        if (knight.GetWorldPos().x < 0.f ||
            knight.GetWorldPos().y < 0.f ||
            knight.GetWorldPos().x + windowDimemsion > mapBG.width * aScale ||
            knight.GetWorldPos().y + windowDimemsion > mapBG.height * aScale)
        {
            // undo movement if out of bounds for current frame
            knight.undoMovement();
        }

        //DrawRectangleLines(knight.GetWorldPos().x, knight.GetWorldPos().y, 100.f, 100.f, BLUE);

        // check if we're colliding with props
        for (auto prop : props)
        {
            if (CheckCollisionRecs(prop.GetCollisionRec(knight.GetWorldPos()), knight.GetCollisionRec()))
            {
                knight.undoMovement();
            }
        }

        // move enemies
        for (int i = 0; i < enemies.size(); i++)
        {
            enemies.at(i)->Tick(GetFrameTime());
        }


        // knight attack
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            // removing an enemy from the vector
            enemies.erase(std::remove_if(
                enemies.begin(),
                enemies.end(),
                // lambda function to check if the sword hits enemy at index in list
                [&knight](Enemy* enemy) { return CheckCollisionRecs(enemy->GetCollisionRec(), knight.getWeaponRect()); }
                ), enemies.end());
        }

        // Debugging to make sure that enemies alive tracking properly
        //DrawText(TextFormat("%.i enemies remain", enemies.size()), knight.GetScreenPos().x, knight.GetScreenPos().y, 22, BLUE);

        // Stylized Health Bar NEEDS OPTIMIZATION
        DrawPlayerHealth(knight);

        // Win game state
        // Vector is empty
        if (enemies.empty())
        {
            int fontsize = 40;
            DrawText("WINNER!", (windowDimemsion / 2.f) - (MeasureText("WINNER!", fontsize) / 2.f) - 2.f, (windowDimemsion / 2.f)  + 2.f, fontsize + 1.f, BLACK);
            DrawText("WINNER!", (windowDimemsion / 2.f) -  MeasureText("WINNER!", fontsize) / 2.f, windowDimemsion / 2.f, fontsize, GOLD);
        }

        EndDrawing();
    }
    knight.unloadAllTexts();
    UnloadTexture(mapBG);
}