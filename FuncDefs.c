#include <stdio.h>
#include <stdlib.h>

#include "./Typedefs.h"
#include "FuncDefs.h"
#include "InitFuncs.h"
#include "IncludeHeader.h"

void drawBlocks(GameObject* gameObject) {
    for (unsigned level = 0; level < NUM_OF_LEVELS; level++) {
        int numOfBlocks = BLOCK_COUNT;

        for (int row = 0; row < BLOCK_ROWS; row++)
            for (int column = 0; column < BLOCK_COUNT; column++)
                if (BLOCKS[row][column]->active)
                    al_draw_filled_rectangle(
                        BLOCKS[row][column]->x1,
                        BLOCKS[row][column]->y1,
                        BLOCKS[row][column]->x2,
                        BLOCKS[row][column]->y2,
                        BLOCKS[row][column]->color
                    );
    }
}

int ballBlockCollision(Ball* ball, Block* block) {
    int returnResult = 0;

    if (BALL_LEFT >= block->x1 && BALL_LEFT <= block->x2)
        if ((ball->cy >= block->y1) && ball->cy <= block->y2)
            if (block->active) {
                ball->dx *= -1;
                returnResult = 1;
            }

    if (BALL_RIGHT >= block->x1 && BALL_RIGHT <= block->x2)
        if ((ball->cy >= block->y1) && ball->cy <= block->y2)
            if (block->active) {
                ball->dx *= -1;
                returnResult = 1;
            }

    if (BALL_TOP >= block->y1 && BALL_TOP <= block->y2)
        if (ball->cx >= block->x1 && ball->cx <= block->x2)
            if (block->active) {
                ball->dy *= -1;
                returnResult = 1;
            }

    if (BALL_DOWN >= block->y1 && BALL_DOWN <= block->y2)
        if (ball->cx >= block->x1 && ball->cx <= block->x2)
            if (block->active) {
                ball->dy *= -1;
                returnResult = 1;
            }

    return returnResult;
}

void blocksRemoval(GameObject* gameObject) {
    for (int row = 0; row < BLOCK_ROWS; row++)
        for (int column = 0; column < BLOCK_COUNT; column++)
            if (ballBlockCollision(gameObject->ball, BLOCK_COL->blocks[row][column])) {
                if (!(--BLOCKS[row][column]->hitPoints)) {
                    if(BLOCKS[row][column]->blockType != 3)
                        BLOCKS[row][column]->active = 0;
                    if ((++LEVEL[gameObject->currentLevel]->blocksCleared) == 
                        (BLOCK_COUNT * BLOCK_ROWS - LEVEL[gameObject->currentLevel]->impenetrableCount))
                        LEVEL_CLEAR = 1;
                    switch (BLOCKS[row][column]->blockType) {
                        case Hard:
                            gameObject->score += 150;
                            break;
                        case Medium:
                            gameObject->score += 100;
                            break;
                        case Soft:
                            gameObject->score += 50;
                            break;
                        case Impenetrable:
                            break;
                    }
                }
            }

}

int ballRectCollision(GameObject* gameObject) {
    Ball* ball = gameObject->ball;
    Rectangle* rect = gameObject->rect;

    if ((BALL_DOWN > rect->y1) && ((BALL_RIGHT < rect->x1) || (BALL_LEFT > rect->x2)))
        return 0;
    else
        return 1;
}

void moveTheBall(GameObject* gameObject) {
    Ball* ball = gameObject->ball;
    Rectangle* rect = gameObject->rect;

    ball->cx += ball->dx;
    ball->cy += ball->dy;

    if ((ball->cx - ball->radius) < 0)
        ball->dx *= -1;

    if ((ball->cx + ball->radius) > SCREEN_WIDTH) {
        ball->cx -= (ball->cx - SCREEN_WIDTH + ball->radius);
        ball->dx *= -1;
    }

    if ((ball->cy - ball->radius) < 0)
        ball->dy *= -1;

    if(ball->cy < (SCREEN_HEIGHT/2))
        blocksRemoval(gameObject);

    // No collision, spawn the ball at the middle
    if ((ball->cy + ball->radius) > rect->y1) {
        if (ballRectCollision(gameObject))
            ball->dy *= -1;
        else
            respawnBall(gameObject);
    }
}

void getLevelString(char* string, int level){
    if(level < 0)
        level = 0;
    
    if(level >= 100)
        level = 100;

    sprintf(string, "Level: %d", level);
}

void getLivesString(char* string, int lives){
    if(lives < 0)
        lives = 0;

    if(lives >= 100)
        lives = 100;

    sprintf(string, "Lives: %d", lives);
}

void getScoreString(char scoreString[], int score){
    if(score < 0)
        score = 0;

    sprintf(scoreString, "Score: %d", score);
}