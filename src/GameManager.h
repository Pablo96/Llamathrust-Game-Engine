#include "Common.h"
#include "Game.h"

typedef struct _GameState {
    const char* callerGame;
    Game* currentGame;
    void* custom_data;
} GameState;

void LT_GameStateInit();
void LT_GameStateUpdateCurrent(const decimal deltaTime);
