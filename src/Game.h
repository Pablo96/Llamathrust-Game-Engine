#include "Common.h"

typedef struct _Game {
    const char* idName;
} Game;

extern void LT_GameOnCreate(Game* this, const char* callerGame, const void* stateData);
extern void LT_GameOnResume(Game* this);
extern void LT_GameOnUpdate(Game* this, const decimal deltaTime);
extern void LT_GameOnStop(Game* this);
extern void LT_GameOnDestroy(Game* this, void* stateData);
