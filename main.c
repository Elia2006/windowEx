#include <stdio.h>
#include "stdlib.h"
#include "raylib.h"
#include "raymath.h"

#include "quadtree.h"
#include "ball.h"

void drawQuadtree(struct Quadtree *mainQuadtree) { 
    DrawRectangleLines(mainQuadtree->pos.x, mainQuadtree->pos.y, mainQuadtree->widthHeight.x, mainQuadtree->widthHeight.y, mainQuadtree->color);
    if(mainQuadtree->ballsHead != NULL) {
        struct Node *temp = mainQuadtree->ballsHead;
        /*while(temp != NULL) {
            DrawCircle(temp->value->pos.x, temp->value->pos.y, 10, mainQuadtree->color);
            temp = temp->next;
        }*/
    }
    if(mainQuadtree->child1 == 0) return;
    drawQuadtree(mainQuadtree->child1);
    drawQuadtree(mainQuadtree->child2);
    drawQuadtree(mainQuadtree->child3);
    drawQuadtree(mainQuadtree->child4);
}

int main(void)
{

    const int screenWidth = 1800;
    const int screenHeight = 1000;


    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(300);


    Vector2 acc = {0, 0};
    float accValue = 2;

    float friction = 50;

    struct Node *head = createBalls(screenWidth, screenHeight);
    
    struct Quadtree *mainQuadtree = malloc(sizeof(struct Quadtree));
    while (!WindowShouldClose())
    {
        mainQuadtree = createQuadtree(0, 0, 1800, 1800, 4);
        mainQuadtree->color = BLACK;
        struct Node *temp = NULL;
        if(head != NULL && head->next != NULL) {
            temp = head->next;
        }
        while(temp != NULL) {
            quadtreeInsert(mainQuadtree, temp->value);
            temp = temp->next;
        }

        updateAcc(&acc, accValue);

        //apply acc
        head->next->value->vel = Vector2Add(head->next->value->vel, Vector2Scale(acc, 1));

        BeginDrawing();

            ClearBackground(RAYWHITE);

            applyVelocity(head, friction);

            handleColision(head, screenWidth, screenHeight);

            drawQuadtree(mainQuadtree);

            DrawText(TextFormat("FPS: %i", (int)(1.0f / GetFrameTime())), 10, 10, 20, RED);

        EndDrawing();
    }
 
    CloseWindow();


    return 0;
}

