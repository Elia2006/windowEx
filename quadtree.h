#include "raylib.h"
#include "raymath.h"

struct Ball;

struct Quadtree {
    Vector2 pos;
    Vector2 widthHeight;
    int capacity;

    struct Node *ballsHead;
    struct Node *ballsCurrent;
    int ballsLength;

    Color color;

    struct Quadtree *child1;
    struct Quadtree *child2;
    struct Quadtree *child3;
    struct Quadtree *child4;
};

void quadtreeInsert(struct Quadtree *main, struct Ball *ball);
struct Quadtree *createQuadtree(int posX, int posY, int width, int height, int capacity);
