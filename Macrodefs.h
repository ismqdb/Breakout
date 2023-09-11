#ifndef MACRODEFS_H
#define MACRODEFS_H

#include "./IncludeHeader.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define FRAME_RATE 60.0

#define KEY_SEEN     1
#define KEY_RELEASED 2

#define BALL_START_X 400
#define BALL_START_Y 530

#define BLOCK_COUNT 20
#define BLOCK_ROWS 4

#define BALL_LEFT   (ball->cx - ball->radius)
#define BALL_RIGHT  (ball->cx + ball->radius)
#define BALL_TOP    (ball->cy - ball->radius)
#define BALL_DOWN   (ball->cy + ball->radius)
#define RAND_COLOR  al_map_rgb(randBetween(0, 255), randBetween(0, 255), randBetween(0, 255))

#define WHITE_COLOR     al_map_rgb(255, 255, 255)
#define RED_COLOR       al_map_rgb(0xf6, 0x41, 0x2D);
#define ORANGE_COLOR    al_map_rgb(0xff, 0x98, 0x00);
#define YELLOW_COLOR    al_map_rgb(0xff, 0xec, 0x19);
#define GREY_COLOR      al_map_rgb(0xaa, 0xaa, 0xaa);

#define NUM_OF_LEVELS 3
#define NUM_OF_LIVES 9

#define HARD_BRICK_ROWS gameObject->levels->blockCollection->hardBricksRows
#define MED_BRICK_ROWS gameObject->levels->blockCollection->mediumBricksRows
#define SOFT_BRICK_ROWS gameObject->levels->blockCollection->softBricksRows

#define BLOCK_COL  gameObject->levels->blockCollection
#define BLOCKS BLOCK_COL->blocks
#define LEVEL gameObject->levels->level
#define LEVEL_CLEAR gameObject->levels->levelCleared

#endif // !Typedefs.H
