#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "snake.h"
#include "board.h"
#include "food.h"


#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 700
/*
TODO: create function createSnake and drawSnake
        write the functions in snake.c and snake.h file
        link the snake.c and snake.h in the make file

TODO: write the handle inputs
TODO: check if you need the SDL_Surface and textrs
*/

struct game
{
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
    Snake *pSnake;
    Board *pBoard;
    Food *pFood;
    // SDL_Rect gameRect;
    int score;
};
typedef struct game Game;

int initiate(Game *pGame);
void run_game(Game *pGame);
void close(Game *pGame);
void handleInput(Game *pGame, SDL_Event *pEvent);


int checkCollision(SDL_Rect pSnake, SDL_Rect pFood);

int main(int argc, char *argv[])
{

    Game g = {0};
    if (!initiate(&g)) return 1;
    run_game(&g);
    close(&g);
    return 0;
}

int initiate(Game *pGame)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0){
        printf("Error initiateing VIDEO or TIMER %s\n", SDL_GetError());
        return 0;
    }
    pGame->pWindow = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!pGame->pWindow){
        printf("Error creating the window: %s\n", SDL_GetError());
        close(pGame);
        return 0;
    }
    Uint32 renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    pGame->pRenderer = SDL_CreateRenderer(pGame->pWindow, -1, renderer_flags);
    if (!pGame->pRenderer)
    {
        printf("Error creating renderer: %s\n", SDL_GetError());
        close(pGame);
        return 0;
    }

    pGame->pBoard = createBoard(pGame->pRenderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    pGame->pSnake = createSnake(pGame->pRenderer, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    pGame->pFood = createFood(pGame->pRenderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    pGame->score = 0;

    return 1;
}
void run_game(Game *pGame)
{
    int close_requested = 0;
    SDL_Event eventGame;
    

    while (!close_requested){
        printf("Running game\n");
        while (SDL_PollEvent(&eventGame)){
            printf("Polling event\n");
            if (eventGame.type == SDL_QUIT) close_requested = 1;
            else handleInput(pGame, &eventGame);
        }
        updateSnake(pGame->pSnake, getBoardWidth(pGame->pBoard), getBoardHeight(pGame->pBoard), getBoardX(pGame->pBoard), getBoardY(pGame->pBoard));
        SDL_Rect snakeHead = pGame->pSnake->pHead->rect;
        SDL_Rect foodRect = pGame->pFood->rect;
        if(checkCollision(snakeHead, foodRect)){
            printf("Collision detected\n");
            growSnake(pGame->pSnake);
            repositionFood(pGame->pFood, WINDOW_WIDTH, WINDOW_HEIGHT);
            pGame->score++;
            printf("Score: %d\n", pGame->score);
            

        }
        // printf("Snake x: %d, y: %d\n", pGame->pSnake->pHead->rect.x, pGame->pSnake->pHead->rect.y);

        SDL_SetRenderDrawColor(pGame->pRenderer, 0, 0, 0, 255);
        SDL_RenderClear(pGame->pRenderer);



        printf("Drawing board\n");
        // SDL_SetRenderDrawColor(pGame->pRenderer, 255, 255, 255, 255);
        drawBoard(pGame->pBoard);
        drawSnake(pGame->pSnake);
        drawFood(pGame->pFood);

        SDL_RenderPresent(pGame->pRenderer);
        SDL_Delay(16);
    }
}
void handleInput(Game *pGame, SDL_Event *pEvent)
{
    if (pEvent->type == SDL_KEYDOWN)
    {
        switch (pEvent->key.keysym.scancode)
        {
        case SDL_SCANCODE_W:
        case SDL_SCANCODE_UP:
            accelerate(pGame->pSnake);
            break;
        case SDL_SCANCODE_A:
        case SDL_SCANCODE_LEFT:
            turnLeft(pGame->pSnake);
            break;
        case SDL_SCANCODE_D:
        case SDL_SCANCODE_RIGHT:
            turnRight(pGame->pSnake);
            break;
        case SDL_SCANCODE_S:
        case SDL_SCANCODE_DOWN:
            accelerateDown(pGame->pSnake);
            break;
        }
    }
}

int checkCollision(SDL_Rect pSnake, SDL_Rect pFood)
{
    return SDL_HasIntersection(&pSnake, &pFood);
}

void close(Game *pGame)
{
    if (pGame->pRenderer) SDL_DestroyRenderer(pGame->pRenderer);
    if (pGame->pWindow) SDL_DestroyWindow(pGame->pWindow);
    SDL_Quit();
}