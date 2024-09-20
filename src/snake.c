#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_error.h>
#include "snake.h"
#include <math.h>
#include <stdio.h>


// static float distance(int x1, int y1, int x2, int y2);

Snake *createSnake(SDL_Renderer *pRenderer, int window_width, int window_height, int x, int y)
{
    Snake *pSnake = malloc(sizeof(struct snake));
    if (!pSnake)
    {
        printf("Failed to allocate memory for snake\n");
        return NULL;
    }
    SnakeSegment *segment = malloc(sizeof(SnakeSegment));
    if (!segment)
    {
        printf("Failed to allocate memory for snake segment\n");
        free(pSnake);
        return NULL;
    }
    segment->rect.h = 20;
    segment->rect.w = 20;
    segment->rect.y = y;
    segment->rect.x = x;
    segment->pNext = NULL;

    pSnake->pHead = segment;
    pSnake->pTail = segment;
    pSnake->length = 1;

    pSnake->vx = 0;
    pSnake->vy = 0;
    pSnake->pRenderer = pRenderer;

    return pSnake;
}

void drawSnake(Snake *pSnake)
{
    SnakeSegment *current = pSnake->pHead;
    SDL_SetRenderDrawColor(pSnake->pRenderer, 0, 255, 0, 255);
    while (current != NULL)
    {
        SDL_RenderFillRect(pSnake->pRenderer, &current->rect);
        current = current->pNext;
    }
}

void turnLeft(Snake *pSnake)
{
    // if (pSnake->vx == 0)
    // {
        pSnake->vx = -5;
        pSnake->vy = 0;
    // }
}

void turnRight(Snake *pSnake)
{
    // if (pSnake->vx == 0)
    // {
        pSnake->vx = 5;
        pSnake->vy = 0;
    // }
}

void accelerate(Snake *pSnake)
{

    // if (pSnake->vy == 0)
    // {
        pSnake->vy = -5;
        pSnake->vx = 0;
    // }
}
void accelerateDown(Snake *pSnake)
{
    // if (pSnake->vy == 0)
    // {
        pSnake->vy = 5;
        pSnake->vx = 0;
    // }
}

int updateSnake(Snake *pSnake, int board_width, int board_height, int board_x, int board_y)
{
    // Save the previous position of the head
    int prev_x = pSnake->pHead->rect.x;
    int prev_y = pSnake->pHead->rect.y;

    // Move the head according to its velocity
    pSnake->pHead->rect.x += pSnake->vx;
    pSnake->pHead->rect.y += pSnake->vy;

    // Loop through the rest of the snake body and move each segment to the previous segment's position
    SnakeSegment *current = pSnake->pHead->pNext;
    while (current != NULL)
    {
        int temp_x = current->rect.x;
        int temp_y = current->rect.y;

        current->rect.x = prev_x;
        current->rect.y = prev_y;

        prev_x = temp_x;
        prev_y = temp_y;

        current = current->pNext;
    }

    if (pSnake->pHead->rect.x < board_x + 1 || pSnake->pHead->rect.x > board_width + 25 ||
        pSnake->pHead->rect.y < board_y + 1 || pSnake->pHead->rect.y > board_height + 25)
    {
        pSnake->vx = 0;
        pSnake->vy = 0;
        // return 0;
    }

    // // Check for board boundaries
    // if (pSnake->pHead->rect.x < board_x || pSnake->pHead->rect.x >= board_x + board_width ||
    //     pSnake->pHead->rect.y < board_y || pSnake->pHead->rect.y >= board_y + board_height) {
    //     printf("Snake hit the boundary!\n");
    //     // Add game over logic or boundary handling here
    // }
}

void growSnake(Snake *pSnake)
{
    SnakeSegment *newSegment = malloc(sizeof(SnakeSegment));
    if (!newSegment)
    {
        printf("Failed to allocate memory for new snake segment\n");
        return;
    }
    // printf("growing snake\n");
    newSegment->rect.w = 20;
    newSegment->rect.h = 20;
    newSegment->pNext = NULL;

    // x y for tail
    newSegment->rect.x = pSnake->pTail->rect.x;
    newSegment->rect.y = pSnake->pTail->rect.y;

    // link tail
    pSnake->pTail->pNext = newSegment;
    pSnake->pTail = newSegment;

    if (!pSnake->pTail)
    {
        printf("Error creating tail: %s\n", SDL_GetError());
    }
    pSnake->length++;
}

// static float distance(int x1, int y1, int x2, int y2)
// {
//     return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
// }

void closeSnake(Snake *pSnake)
{
    SnakeSegment *current = pSnake->pHead;
    while (current != NULL)
    {
        SnakeSegment *temp = current;
        current = current->pNext;
        free(temp);
    }
    free(pSnake);
}

/*
// Let the snake AI know where the food is and move towards it
void moveSnakeAI(Snake *pSnake, int food_x, int food_y) {
    // Get the head of the snake
    SDL_Rect head = pSnake->pHead->rect;

    // Calculate the distance to the food
    float dist = distance(head.x, head.y, food_x, food_y);

    // Determine the direction to move
    if (dist > 0) {
        // Move horizontally towards the food
        if (food_x > head.x) {
            pSnake->vx = 5;
            pSnake->vy = 0;
        } else if (food_x < head.x) {
            pSnake->vx = -5;
            pSnake->vy = 0;
        }
        // Move vertically towards the food
        else if (food_y > head.y) {
            pSnake->vy = 5;
            pSnake->vx = 0;
        } else if (food_y < head.y) {
            pSnake->vy = -5;
            pSnake->vx = 0;
        }
    }

    // Update the snake's position
    updateSnake(pSnake, 1280, 700, 0, 0); // Assuming the board dimensions are 1280x700
}

*/

// Function to calculate the Euclidean distance between two points
float distance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// Let the snake AI know where the food is and move towards it
void moveSnakeAI(Snake *pSnake, int food_x, int food_y, int board_width, int board_height, int board_x, int board_y)
{
    // Get the head of the snake
    SDL_Rect head = pSnake->pHead->rect;

    // Calculate the distance to the food
    float dist_x = food_x - head.x;
    float dist_y = food_y - head.y;
    // make snake ai go to the food and avoid collision with board boundaries
    // if(head.x +5 >board_width || head.x -5 < board_x || head.y +5 > board_height || head.y-5<board_y)
        // avoidCollisionAI(pSnake,  board_width,  board_height,  board_x,  board_y);
    if(head.x > food_x && dist_x != 0)
    {
        turnLeft(pSnake);
    }
    else if(head.x > food_x && dist_x != 0)
    {
        turnRight(pSnake);
    }
    else if(head.y > food_y && dist_y != 0)
    {
        accelerate(pSnake);
    }
    else if(head.y < food_y && dist_y != 0)
    {
        accelerateDown(pSnake);
    }
    
    // avoidCollisionAI(pSnake,  board_width,  board_height,  board_x,  board_y);

    updateSnake(pSnake, 1280, 700, board_x, board_y); // Assuming the board dimensions are 1280x700
}

void avoidCollisionAI(Snake *pSnake, int board_width, int board_height, int board_x, int board_y)
{
    // if (pSnake->pHead->rect.x < board_x + 1 || pSnake->pHead->rect.x > board_width + 25 ||
    //     pSnake->pHead->rect.y < board_y + 1 || pSnake->pHead->rect.y > board_height + 25)
   
    SDL_Rect head = pSnake->pHead->rect;
    // Determine the direction to move
    if (head.x + 5 > board_width+25)
    {
        if(head.y + 5 > board_height-5){
            accelerate(pSnake);
            turnLeft(pSnake);
        }
        else{
            accelerateDown(pSnake);
            turnLeft(pSnake);
        }
        
    }
    else if (head.x - 5 < board_x)
    {
        if(head.y - 5 < board_y){
            accelerateDown(pSnake);
            turnRight(pSnake);
        }
        else{
            accelerate(pSnake);
            turnRight(pSnake);
        }
    }
    else if (head.y + 5 > board_height-25)
    {
        if(head.x + 5 > board_width+25){
            turnLeft(pSnake);
            accelerate(pSnake);
        }
        else{
            turnRight(pSnake);
            accelerate(pSnake);
        }
    }
    else if (head.y - 5 < board_y)
    {
        if(head.x + 5 > board_width+25){
            turnLeft(pSnake);
            accelerateDown(pSnake);
        }
        else{
            turnRight(pSnake);
            accelerateDown(pSnake);
        }
    }
}

