#include "GameManager.h"
#include <stdlib.h>

static GameState gameState;

void LT_GameStateInit(const char* in_gameName) {
    gameState.currentGame = NULL;
    gameState.callerGame  = NULL;
    gameState.customData  = NULL;
}

void LT_GameStateUpdateCurrent(const double deltaTime) {

}