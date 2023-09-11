#include "InitFuncs.h"
#include "IncludeHeader.h"

#include <stdio.h>

int randBetween(int lo, int hi) {
    return rand() % (lo + 1 - hi) + lo;
}

void mustInit(bool test, const char* description)
{
    if (test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

void InitFunctions(GameObject* gameObject) {
    mustInit(al_init(), "allegro");
    mustInit(al_install_keyboard(), "keyboard");

    gameObject->timer = al_create_timer(1.0 / FRAME_RATE);
    mustInit(gameObject->timer, "timer");

    gameObject->eventQueue = al_create_event_queue();
    mustInit(gameObject->eventQueue, "queue");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    gameObject->display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    mustInit(gameObject->display, "display");

    gameObject->font = al_create_builtin_font();
    mustInit(gameObject->font, "font");

    mustInit(al_init_primitives_addon(), "primitives");

    mustInit(al_install_audio(), "audio");
    mustInit(al_init_acodec_addon(), "audio codecs");
    mustInit(al_reserve_samples(16), "reserve samples");

    al_register_event_source(gameObject->eventQueue, al_get_keyboard_event_source());
    al_register_event_source(gameObject->eventQueue, al_get_display_event_source(gameObject->display));
    al_register_event_source(gameObject->eventQueue, al_get_timer_event_source(gameObject->timer));
}

void initGameObject(GameObject* gameObject) {
    gameObject->rect = (Rectangle*)malloc(sizeof(Rectangle));
    gameObject->ball = (Ball*)malloc(sizeof(Ball));
    gameObject->levels = (Levels*)malloc(sizeof(Levels));

    for (unsigned level = 0; level < NUM_OF_LEVELS; level++) {
        LEVEL[level] = (Level*)malloc(sizeof(Level));
        BLOCK_COL = (BlockCollection*)malloc(sizeof(BlockCollection));
    }

    for (int row = 0; row < BLOCK_ROWS; row++)
        for(int column = 0; column < BLOCK_COUNT; column++)
            BLOCKS[row][column] = (Block*)malloc(sizeof(Block));

    gameObject->currentLevel = -1;
    gameObject->score = 0;
    gameObject->gameRunning = false;

    gameObject->event = (ALLEGRO_EVENT*)malloc(sizeof(ALLEGRO_EVENT));
    gameObject->keyboardState = (ALLEGRO_KEYBOARD_STATE*)malloc(sizeof(ALLEGRO_KEYBOARD_STATE));
}

void initRect(GameObject* gameObject) {
    Rectangle* rect = gameObject->rect;

    gameObject->rect->x1 = 350;
    rect->y1 = 540;
    rect->x2 = 450;
    rect->y2 = 550;
    rect->dx = 15;
}

void initBall(GameObject* gameObject) {
    Ball* ball = gameObject->ball;

    ball->cx = BALL_START_X;
    ball->cy = BALL_START_Y;
    ball->radius = 10;
    ball->dx = 5;
    ball->dy = -5;
    ball->lives = NUM_OF_LIVES;
}

void respawnBall(GameObject* gameObject) {
    Ball* ball = gameObject->ball;

    al_rest(1);
    ball->lives--;
    ball->cx = BALL_START_X;
    ball->cy = BALL_START_Y;
    ball->dx = 5 * randBetween(-1, 1);
    ball->dy = -5;
    ball->isActive = 0;
    initRect(gameObject);
}

void initLevel(GameObject* gameObject) {
    gameObject->currentLevel++;
    gameObject->levels->levelCleared = 0;
    LEVEL[gameObject->currentLevel]->blocksCleared = 0;
    LEVEL[gameObject->currentLevel]->impenetrableCount = 0;

    HARD_BRICK_ROWS = BLOCK_ROWS * 50 / 100;
    MED_BRICK_ROWS = (BLOCK_ROWS - HARD_BRICK_ROWS) * 75 / 100;
    SOFT_BRICK_ROWS = BLOCK_ROWS - HARD_BRICK_ROWS - MED_BRICK_ROWS;

    initBlocks(gameObject);
}

void initBlocks(GameObject* gameObject) {
    unsigned hardBricksRows = HARD_BRICK_ROWS;
    unsigned medBricksRows = MED_BRICK_ROWS;
    unsigned softBricksRows = SOFT_BRICK_ROWS;

    BlockType hardBrick = Hard;
    BlockType medBrick = Medium;
    BlockType softBrick = Soft;

    double blockWidth = 25;
    double blockHeight = 15;

    double blankWidth = (800 - BLOCK_COUNT * 25) / (BLOCK_COUNT + 1);
    double blankHeight = blockHeight / 2;
    double currentXPos = 0;
    double currentYPos = 100;

    for (int row = 0; row < BLOCK_ROWS; row++) {
        for (int column = 0; column < BLOCK_COUNT; column++) {
            currentXPos += blankWidth;
            BLOCKS[row][column]->x1 = currentXPos;
            BLOCKS[row][column]->x2 = currentXPos + blockWidth;
            BLOCKS[row][column]->y1 = currentYPos;
            BLOCKS[row][column]->y2 = currentYPos + blockHeight;
            BLOCKS[row][column]->active = true;

            // init colors and hit points            
            if (hardBricksRows) {
                BLOCKS[row][column]->color = RED_COLOR;
                BLOCKS[row][column]->hitPoints = 3;
                BLOCKS[row][column]->breakScore = 150;
                BLOCKS[row][column]->blockType = hardBrick;
            }
            else if (medBricksRows) {
                BLOCKS[row][column]->color = ORANGE_COLOR;
                BLOCKS[row][column]->hitPoints = 2;
                BLOCKS[row][column]->breakScore = 100;
                BLOCKS[row][column]->blockType = medBrick;
            }
            else if (softBricksRows) {
                BLOCKS[row][column]->color = YELLOW_COLOR;
                BLOCKS[row][column]->hitPoints = 1;
                BLOCKS[row][column]->breakScore = 50;
                BLOCKS[row][column]->blockType = softBrick;
            }    

            currentXPos += blockWidth;
        }

        // spawn random impenetrable block
        int y = randBetween(0, BLOCK_COUNT);
        BLOCKS[row][y]->blockType = Impenetrable;
        BLOCKS[row][y]->color = GREY_COLOR;
        BLOCKS[row][y]->active = 1;
        LEVEL[gameObject->currentLevel]->impenetrableCount++;  
        BLOCKS[row][y]->hitPoints = -1;


        if (hardBricksRows)
            hardBricksRows--;
        else if (medBricksRows) 
            medBricksRows--;
        else if (softBricksRows)
            softBricksRows--;        

        currentYPos += blockHeight;
        currentYPos += blankHeight;
        currentXPos = 0;
    }

}