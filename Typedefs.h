#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include "Macrodefs.h"

typedef enum BlockType {
    Hard,
    Medium, 
    Soft,
    Impenetrable
} BlockType;

typedef struct Rectangle {
    int x1, x2;
    int y1, y2;
    int dx;
} Rectangle;

typedef struct Ball {
    int cx;
    int cy;
    int radius;
    int dx;
    int dy;
    unsigned int lives;
    ALLEGRO_COLOR* color;
    int isActive;
} Ball;

typedef struct Block {
    int x1;
    int y1;
    int x2;
    int y2;
    int active;
    unsigned hitPoints;
    unsigned breakScore;
    BlockType blockType;
    ALLEGRO_COLOR color;
} Block;

typedef struct BlockCollection {
    Block* blocks[BLOCK_ROWS][BLOCK_COUNT];
    unsigned hardBricksRows;
    unsigned mediumBricksRows;
    unsigned softBricksRows;
} BlockCollection;

typedef struct Level {
    unsigned rowCount;
    unsigned columnCount;
    unsigned rowSpacing;
    unsigned columnSpacing;
    unsigned blocksCleared;
    unsigned impenetrableCount;
} Level;

typedef struct Levels {
    Level* level[NUM_OF_LEVELS];
    BlockCollection* blockCollection;
    unsigned levelCleared;
} Levels;

typedef struct GameObject {
    Rectangle* rect;
    Ball* ball;
    Levels* levels;
    ALLEGRO_TIMER* timer;
    ALLEGRO_EVENT_QUEUE* eventQueue;
    ALLEGRO_DISPLAY* display;
    ALLEGRO_FONT* font;
    ALLEGRO_EVENT* event;
    ALLEGRO_KEYBOARD_STATE* keyboardState;
    unsigned char key[ALLEGRO_KEY_MAX];
    int done;
    int redraw;
    unsigned currentLevel;
    unsigned score;
    bool gameRunning;
} GameObject;

#endif // !Typedefs.H
