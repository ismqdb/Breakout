#ifndef INITFUNCS_H
#define INITFUNCS_H

#include "./Typedefs.h"
#include "IncludeHeader.h"

void mustInit(bool test, const char* description);
void InitFunctions(GameObject*);
void initRect(GameObject*);
void initBall(GameObject*);
void initBlocks(GameObject* gameObject);
void initGameObject(GameObject* gameObject);
void initLevel(GameObject* gameObject);
int randBetween(int lo, int hi);

#endif // !InitFuncs
