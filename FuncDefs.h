#ifndef FUNCDEFS_H
#define FUNCDEFS_H

#include "./Typedefs.h"

void drawBlocks(GameObject*);
int ballBlockCollision(Ball* ball, Block* block);
void blocksRemoval(GameObject* gameObject);
int ballRectCollision(GameObject*);
void moveTheBall(GameObject*);
void respawnBall(GameObject* gameObject);
void getLevelString(char*, int);
void getLivesString(char* string, int lives);
void getScoreString(char[], int);

#endif // !FuncDefs
