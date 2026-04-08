#include <stdio.h>
#include "stdlib.h"
#include "raylib.h"
#include "raymath.h"

#include "quadtree.h"
#include "ball.h"

struct Quadtree *createQuadtree(int posX, int posY, int width, int height, int capacity) {
    struct Quadtree *newQuadtree = malloc(sizeof(struct Quadtree));
    newQuadtree->pos.x = posX;
    newQuadtree->pos.y = posY;
    newQuadtree->widthHeight.x = width;
    newQuadtree->widthHeight.y = height;
    newQuadtree->ballsLength = 0;
    newQuadtree->ballsHead = NULL;
    newQuadtree->ballsCurrent = NULL;
    newQuadtree->capacity = capacity;
    newQuadtree->child1 = NULL;
    newQuadtree->child2 = NULL;
    newQuadtree->child3 = NULL;
    newQuadtree->child4 = NULL;

    return newQuadtree;
}

void quadtreeInsert(struct Quadtree *main, struct Ball *ball) {

    if(ball->pos.x < main->pos.x || ball->pos.y < main->pos.y ||
        ball->pos.x > main->pos.x + main->widthHeight.x || ball->pos.y > main->pos.y + main->widthHeight.y) return;

    if(main->ballsLength < main->capacity) {
        struct Node *newCurrent = malloc(sizeof(struct Node));
        newCurrent->value = ball;
        newCurrent->next = NULL;

        if(main->ballsHead == NULL) {
            main->ballsHead = newCurrent;
            main->ballsCurrent = main->ballsHead;
        } else {
            main->ballsCurrent->next = newCurrent;
            main->ballsCurrent = newCurrent;
        }
        main->ballsLength++;
    } else{
        if(main->child1 == NULL) {
            main->child1 = createQuadtree(
                main->pos.x, main->pos.y, 
                main->widthHeight.x / 2, main->widthHeight.y / 2, 
                main->capacity);
            
            main->child2 = createQuadtree(
                main->pos.x + main->widthHeight.x / 2, main->pos.y, 
                main->widthHeight.x / 2, main->widthHeight.y / 2,
                main->capacity);

            main->child3 = createQuadtree(
                main->pos.x, main->pos.y + main->widthHeight.y / 2, 
                main->widthHeight.x / 2, main->widthHeight.y / 2, 
                main->capacity);

            main->child4 = createQuadtree(
                main->pos.x + main->widthHeight.x / 2, main->pos.y + main->widthHeight.y / 2, 
                main->widthHeight.x / 2, main->widthHeight.y / 2, 
                main->capacity);

            struct Node *temp = main->ballsHead;
            while(temp != NULL) {
                quadtreeInsert(main->child1, temp->value);
                quadtreeInsert(main->child2, temp->value);
                quadtreeInsert(main->child3, temp->value);
                quadtreeInsert(main->child4, temp->value);

                struct Node *next = temp->next;
                free(temp);
                temp = next;
            }
            
            main->child1->color = RED;
            main->child2->color = GREEN;
            main->child3->color = BLUE;
            main->child4->color = BLACK;
        }

        quadtreeInsert(main->child1, ball);

        quadtreeInsert(main->child2, ball);
        
        quadtreeInsert(main->child3, ball);

        quadtreeInsert(main->child4, ball);
        
    }
}