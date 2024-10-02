#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_ttf.h>
#include <direct.h>

#include <time.h>
#include "snake.h"
#include "board.h"
#include "food.h"


#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 700
#define FONT_SRC "./resources/arial.ttf"
#define FONT_SIZE 64
#define HIGHSCORE_FILE "./resources/highscores.txt"


typedef struct highscore {
    char name[50];
    int score;
} HighScore;

struct game
{
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
    Snake *pSnake;
    Snake *pSnakeAI;
    Board *pBoard;
    Food *pFood;
    TTF_Font *font;
    SDL_Texture *scoreTexture;
    SDL_Texture *aiScoreTexture;
    int scoreTextWidth;
    int scoreTextHeight;
    int aiScoreTextWidth;
    int aiScoreTextHeight;
};
typedef struct game Game;

int initiate(Game *pGame);
void run_game(Game *pGame);
void close_game(Game *pGame);
void handleInput(Game *pGame, SDL_Event *pEvent);
void mainMenu(Game *pGame);
void showHighScore(Game *pGame);
void saveScore(const char *playerName, int score);
void drawMenu(Game *pGame);
void updateScore(Snake *pSnake, Game *pGame, int isAi);


int checkCollision(SDL_Rect pSnake, SDL_Rect pFood);

int main(int argc, char *argv[])
{

    Game g = {0};
    if (!initiate(&g)) return 1;
    drawMenu(&g);
    close_game(&g);
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
        close_game(pGame);
        return 0;
    }
    Uint32 renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    pGame->pRenderer = SDL_CreateRenderer(pGame->pWindow, -1, renderer_flags);
    if (!pGame->pRenderer)
    {
        printf("Error creating renderer: %s\n", SDL_GetError());
        close_game(pGame);
        return 0;
    }
    if (TTF_Init() < 0) {
        printf("SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());
        close_game(pGame); 
    }

    pGame->pBoard = createBoard(pGame->pRenderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    pGame->pSnake = createSnake(pGame->pRenderer, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    pGame->pSnakeAI = createSnake(pGame->pRenderer, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    pGame->pFood = createFood(pGame->pRenderer, getBoardWidth(pGame->pBoard), getBoardHeight(pGame->pBoard), getBoardX(pGame->pBoard), getBoardY(pGame->pBoard));

    return 1;
}
HighScore* readHighScores(int *numScores) {
    FILE *file = fopen(HIGHSCORE_FILE, "r");
    if (file == NULL) {
        printf("Error opening file for reading\n");
        *numScores = 0;
        return NULL;
    }

    HighScore *scores = malloc(100 * sizeof(HighScore)); 
    *numScores = 0;

    while (fscanf(file, "%49s %d", scores[*numScores].name, &scores[*numScores].score) == 2) {
        (*numScores)++;
    }

    fclose(file);
    return scores;
}
void saveScore(const char *playerName, int score) {
    FILE *file = fopen(HIGHSCORE_FILE, "a");
    if (file == NULL) {
        printf("Error opening file for writing\n");
        return;
    }
    fprintf(file, "%s %d\n", playerName, score);
    fclose(file);
}



void drawMenu(Game *pGame) {



    TTF_Font *font = TTF_OpenFont(FONT_SRC, FONT_SIZE);
    if (!font) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        return;
    }

    SDL_Color white = {255, 255, 255, 255};

    SDL_Rect menuRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_SetRenderDrawColor(pGame->pRenderer, 0, 0, 0, 255);
    SDL_RenderFillRect(pGame->pRenderer, &menuRect);

    SDL_Surface *playSurface = TTF_RenderText_Solid(font, "Play", white);
    SDL_Texture *playTexture = SDL_CreateTextureFromSurface(pGame->pRenderer, playSurface);
    int playTextWidth, playTextHeight;
    SDL_QueryTexture(playTexture, NULL, NULL, &playTextWidth, &playTextHeight);
    SDL_Rect playTextRect = {WINDOW_WIDTH / 2 - playTextWidth / 2, WINDOW_HEIGHT / 2 - 50 + (50 - playTextHeight) / 2, playTextWidth, playTextHeight};
    SDL_RenderCopy(pGame->pRenderer, playTexture, NULL, &playTextRect);

    SDL_Surface *highScoreSurface = TTF_RenderText_Solid(font, "High Scores", white);
    SDL_Texture *highScoreTexture = SDL_CreateTextureFromSurface(pGame->pRenderer, highScoreSurface);
    int highScoreTextWidth, highScoreTextHeight;
    SDL_QueryTexture(highScoreTexture, NULL, NULL, &highScoreTextWidth, &highScoreTextHeight);
    SDL_Rect highScoreTextRect = {WINDOW_WIDTH / 2 - highScoreTextWidth / 2, WINDOW_HEIGHT / 2 + 10 + (50 - highScoreTextHeight) / 2, highScoreTextWidth, highScoreTextHeight};
    SDL_RenderCopy(pGame->pRenderer, highScoreTexture, NULL, &highScoreTextRect);

    SDL_RenderPresent(pGame->pRenderer);

    SDL_FreeSurface(playSurface);
    SDL_FreeSurface(highScoreSurface);
    SDL_DestroyTexture(playTexture);
    SDL_DestroyTexture(highScoreTexture);

    TTF_CloseFont(font);

    

    int close_requested = 0;
    SDL_Event eventGame;

    while (!close_requested) {
        while (SDL_PollEvent(&eventGame)) {
            if (eventGame.type == SDL_QUIT) {
                close_requested = 1;
            } else if (eventGame.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);

                if (x >= playTextRect.x && x <= playTextRect.x + playTextRect.w &&
                    y >= playTextRect.y && y <= playTextRect.y + playTextRect.h) {
                    run_game(pGame);
                    close_requested = 1;
                }

                if (x >= highScoreTextRect.x && x <= highScoreTextRect.x + highScoreTextRect.w &&
                    y >= highScoreTextRect.y && y <= highScoreTextRect.y + highScoreTextRect.h) {
                    showHighScore(pGame);
                    close_requested = 1;
                }
            }
        }
    }
}

void showHighScore(Game *pGame) {
 

    TTF_Font *font = TTF_OpenFont(FONT_SRC, FONT_SIZE);
    if (!font) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        return;
    }

    SDL_Color white = {255, 255, 255, 255};

    SDL_Rect highScoreRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_SetRenderDrawColor(pGame->pRenderer, 0, 0, 0, 255);
    SDL_RenderFillRect(pGame->pRenderer, &highScoreRect);

    int numScores;
    HighScore *scores = readHighScores(&numScores);

    for (int i = 0; i < numScores; ++i) {
        char scoreText[100];
        snprintf(scoreText, sizeof(scoreText), "%s - %d", scores[i].name, scores[i].score);

        SDL_Surface *surface = TTF_RenderText_Solid(font, scoreText, white);
        if (!surface) {
            printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
            TTF_CloseFont(font);
            free(scores);
            return;
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(pGame->pRenderer, surface);
        if (!texture) {
            printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
            SDL_FreeSurface(surface);
            TTF_CloseFont(font);
            free(scores);
            return;
        }

        int textWidth, textHeight;
        SDL_QueryTexture(texture, NULL, NULL, &textWidth, &textHeight);
        SDL_Rect textRect = {WINDOW_WIDTH / 2 - textWidth / 2, 50 + i * 50, textWidth, textHeight};

        SDL_RenderCopy(pGame->pRenderer, texture, NULL, &textRect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    SDL_RenderPresent(pGame->pRenderer);

    int close_requested = 0;
    SDL_Event eventGame;

    while (!close_requested) {
        while (SDL_PollEvent(&eventGame)) {
            if (eventGame.type == SDL_QUIT) {
                close_requested = 1;
            } else if (eventGame.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);

                if (x >= 0 && x <= WINDOW_WIDTH && y >= 0 && y <= WINDOW_HEIGHT) {
                    drawMenu(pGame);
                    close_requested = 1;
                }
            }
        }
    }
}

void endGame(Game *pGame, const char *playerName, int score) {
    saveScore(playerName, score);
    
    
    pGame->pSnake = createSnake(pGame->pRenderer, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    pGame->pSnakeAI = createSnake(pGame->pRenderer, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    SDL_DestroyTexture(pGame->scoreTexture);
    pGame->scoreTexture = NULL;
    SDL_DestroyTexture(pGame->aiScoreTexture);
    pGame->aiScoreTexture = NULL;
    drawMenu(pGame);
}

void run_game(Game *pGame)
{
    srand(time(NULL));

    int close_requested = 0;
    SDL_Event eventGame;
    

    while (!close_requested){
        while (SDL_PollEvent(&eventGame)){
            if (eventGame.type == SDL_QUIT) close_requested = 1;
            else {
                handleInput(pGame, &eventGame);
            }
        }
        if(!updateSnake(pGame->pSnake, getBoardWidth(pGame->pBoard), getBoardHeight(pGame->pBoard), getBoardX(pGame->pBoard), getBoardY(pGame->pBoard))){
            printf("Game over\n");
            break;
        }
        moveSnakeAI(pGame->pSnakeAI, foodX(pGame->pFood), foodY(pGame->pFood), getBoardWidth(pGame->pBoard), getBoardHeight(pGame->pBoard), getBoardX(pGame->pBoard), getBoardY(pGame->pBoard));
        if(!updateSnake(pGame->pSnakeAI, getBoardWidth(pGame->pBoard), getBoardHeight(pGame->pBoard), getBoardX(pGame->pBoard), getBoardY(pGame->pBoard))){
            printf("Game over\n");
            break;
        }
        SDL_Rect snakeHeadRectAI = pGame->pSnakeAI->pHead->rect;
        SDL_Rect snakeHeadRect = pGame->pSnake->pHead->rect;
        SDL_Rect foodRect = pGame->pFood->rect;
        if(checkCollision(snakeHeadRect, foodRect)){
            growSnake(pGame->pSnake);
            repositionFood(pGame->pFood, getBoardWidth(pGame->pBoard), getBoardHeight(pGame->pBoard), getBoardX(pGame->pBoard), getBoardY(pGame->pBoard));
            updateScore(pGame->pSnake, pGame,0);

        }
        else if(checkCollision(snakeHeadRectAI, foodRect)){
            growSnake(pGame->pSnakeAI);
            repositionFood(pGame->pFood, getBoardWidth(pGame->pBoard), getBoardHeight(pGame->pBoard), getBoardX(pGame->pBoard), getBoardY(pGame->pBoard));
            updateScore(pGame->pSnakeAI, pGame, 1);
        }

        SDL_SetRenderDrawColor(pGame->pRenderer, 0, 0, 0, 255);
        SDL_RenderClear(pGame->pRenderer);

        drawBoard(pGame->pBoard);
        drawSnake(pGame->pSnakeAI);
        drawSnake(pGame->pSnake);
        drawFood(pGame->pFood);

        
        if (pGame->aiScoreTexture) {
            SDL_Rect aiTextRect = {10, 10, pGame->aiScoreTextWidth, pGame->aiScoreTextHeight};
            SDL_RenderCopy(pGame->pRenderer, pGame->aiScoreTexture, NULL, &aiTextRect);
        }
        if (pGame->scoreTexture) {
            SDL_Rect userTextRect = {WINDOW_WIDTH - pGame->scoreTextWidth - 10, 10, pGame->scoreTextWidth, pGame->scoreTextHeight};
            SDL_RenderCopy(pGame->pRenderer, pGame->scoreTexture, NULL, &userTextRect);
        }

        SDL_RenderPresent(pGame->pRenderer);
        SDL_Delay(16);
    }
    printf("Game over\n");
    printf("Score: %d\n", pGame->pSnake->length);
    printf("Enter your name: ");
    const char playerName[50];
    scanf("%s", playerName);


    

    endGame(pGame, playerName, pGame->pSnake->length);
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

void close_game(Game *pGame)
{
    if(pGame->font) TTF_CloseFont(pGame->font);
    if (pGame->pRenderer) SDL_DestroyRenderer(pGame->pRenderer);
    if (pGame->pWindow) SDL_DestroyWindow(pGame->pWindow);
    SDL_Quit();
}

void updateScore(Snake *pSnake, Game *pGame, int isAI){
    TTF_Font *font = TTF_OpenFont(FONT_SRC, FONT_SIZE);
    if (!font) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        return;
    }


    SDL_Color white = {255, 255, 255, 255};
    char scoreText[100];
    snprintf(scoreText, sizeof(scoreText), "Score: %d", pSnake->length-1);

    SDL_Surface *surface = TTF_RenderText_Solid(font, scoreText, white);
    if (!surface) {
        printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(pGame->pRenderer, surface);
    if (!texture) {
        printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    if (isAI) {
        pGame->aiScoreTexture = texture;
        SDL_QueryTexture(texture, NULL, NULL, &pGame->aiScoreTextWidth, &pGame->aiScoreTextHeight);
    } else {
        pGame->scoreTexture = texture;
        SDL_QueryTexture(texture, NULL, NULL, &pGame->scoreTextWidth, &pGame->scoreTextHeight);
    }
    
    SDL_FreeSurface(surface);
}