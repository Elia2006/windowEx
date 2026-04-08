#ifndef NODE_H
#define NODE_H

#include "raylib.h"
#include "raymath.h"

struct Node {
    struct Ball *value;
    struct Node *next; 
};

struct Ball {
    Vector2 pos;
    Vector2 vel;

    int mass;

    int size;

    Color color;
};

struct Node *createBalls(int screenWidth, int screenHeight);
void deleteNode(struct Node **head, struct Ball *target);
void updateAcc(Vector2 *acc, float accValue);
void applyVelocity(struct Node *head, float friction);
void handleColision(struct Node *head, int screenWidth, int screenHeight);

#endif