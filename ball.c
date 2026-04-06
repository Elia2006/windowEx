#include "stdlib.h"
#include "raylib.h"
#include "raymath.h"

#include "ball.h"

struct Node *createBalls(int screenWidth, int screenHeight) {
    struct Node *head = NULL;
    struct Node *prevNode = NULL;
    for(int x = 10; x < screenWidth; x += 30) {
        for(int y = 10; y < screenHeight; y += 30) {
            struct Ball *ball = malloc(sizeof(struct Ball));
            ball->pos.x = x;
            ball->pos.y = y;
            ball->vel.x = 0;
            ball->vel.y = 0;
            ball->size = 11;
            ball->mass = 10;
            ball->color = DARKBLUE;

            struct Node *newNode = malloc(sizeof(struct Node));
            newNode->value = ball;
            newNode->next = NULL;

            if(head == NULL) {
                head = newNode;
            } else {
                prevNode->next = newNode;
            }

            prevNode = newNode;
        }
    }

    return head;
}

void deleteNode(struct Node **head, struct Ball *target) {
    struct Node *current = *head;
    struct Node *prev = NULL;

    if(current != NULL && current->value == target) {
        *head = current->next;
        free(current);
        return;
    }

    while(current != NULL && current->value != target) {
        prev = current;
        current = current->next;
    }

    if(current == NULL){
        return;
    }

    prev->next = current->next;

    free(current);

}


void updateAcc(Vector2 *acc, float accValue) {

    if(IsKeyDown(KEY_W)) {
        acc->y = -accValue;
    }

    else if(IsKeyDown(KEY_S)) {
        acc->y = accValue;
    }
    else {
        acc->y = 0;
    }

    if(IsKeyDown(KEY_A)) {
        acc->x = -accValue;
    }
    else if(IsKeyDown(KEY_D)) {
        acc->x = accValue;
    }
    else {
        acc->x = 0;
    }
}

void applyVelocity(struct Node *head, float friction) {
    struct Node *temp = head;
    while(temp != NULL) {
        struct Ball *thisBall = temp->value;


        float speed = Vector2Length(thisBall->vel);
        if(speed > 0.0) {
            speed -= friction * GetFrameTime();
            if(speed < 0) speed = 0;
            thisBall->vel = Vector2Scale(Vector2Normalize(thisBall->vel), speed);
        }

        thisBall->pos.x += thisBall->vel.x * GetFrameTime();
        thisBall->pos.y += thisBall->vel.y * GetFrameTime();

        temp = temp->next;
    }
}

void handleColision(struct Node *head, int screenWidth, int screenHeight) {
    struct Node *temp = head;
    while(temp != NULL) {
        struct Ball *thisBall = temp->value;

        if(thisBall == NULL) continue;

        if(thisBall->pos.x - thisBall->size <= 0) {
            thisBall->vel.x *= -1;
            thisBall->pos.x = thisBall->size;

        }
        else if(thisBall->pos.x + thisBall->size >= screenWidth) {
            thisBall->vel.x *= -1;
            thisBall->pos.x = screenWidth - thisBall->size;
        }
        
        if(thisBall->pos.y - thisBall->size <= 0) {
            thisBall->vel.y *= -1;
            thisBall->pos.y = thisBall->size;
            
        }
        if(thisBall->pos.y + thisBall->size >= screenHeight) {
            thisBall->vel.y *= -1;
            thisBall->pos.y = screenHeight - thisBall->size;
        }

        struct Node *temp2 = temp;
        while(temp2->next != NULL) {
            temp2 = temp2->next;
            struct Ball *nextBall = temp2->value;
            
            double distance = Vector2Distance(thisBall->pos, nextBall->pos);
            
            if(distance < thisBall->size + nextBall->size) {

                Vector2 impactLine = Vector2Subtract(nextBall->pos, thisBall->pos);

                float overlap = (thisBall->size + nextBall->size) - distance;
                thisBall->pos = Vector2Subtract(thisBall->pos, Vector2Scale(Vector2Normalize(impactLine), overlap / 2));
                nextBall->pos = Vector2Add(nextBall->pos, Vector2Scale(Vector2Normalize(impactLine), overlap / 2));


                impactLine = Vector2Subtract(nextBall->pos, thisBall->pos);

                distance = Vector2Distance(thisBall->pos, nextBall->pos);

                double mSum = thisBall->mass + nextBall->mass;

                Vector2 velDiff = Vector2Subtract(nextBall->vel, thisBall->vel);

                float a = Vector2DotProduct(velDiff, impactLine) / (distance * distance);


                thisBall->vel = Vector2Add(thisBall->vel, Vector2Scale(impactLine, 2 * nextBall->mass / mSum * a));
                
                nextBall->vel = Vector2Add(nextBall->vel, Vector2Scale(Vector2Negate(impactLine), 2 * thisBall->mass / mSum * a));
            }
        }
            

        DrawCircle(thisBall->pos.x, thisBall->pos.y, thisBall->size, thisBall->color);

        temp = temp->next;
    }
}