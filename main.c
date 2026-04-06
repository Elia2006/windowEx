#include "stdlib.h"
#include "raylib.h"
#include "raymath.h"

#include "ball.h"



int main(void)
{

    const int screenWidth = 1800;
    const int screenHeight = 1000;


    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(300);


    Vector2 acc = {0, 0};
    float accValue = 10;

    float friction = 50;

    struct Node * head = createBalls(screenWidth, screenHeight);


    while (!WindowShouldClose())
    {
        updateAcc(&acc, accValue);

        //apply acc
        head->next->value->vel = Vector2Add(head->next->value->vel, Vector2Scale(acc, 1));

        BeginDrawing();

            ClearBackground(RAYWHITE);

            applyVelocity(head, friction);

            handleColision(head, screenWidth, screenHeight);

            DrawText(TextFormat("FPS: %i", (int)(1.0f / GetFrameTime())), 10, 10, 20, RED);

        EndDrawing();
    }
 
    CloseWindow();


    return 0;
}

