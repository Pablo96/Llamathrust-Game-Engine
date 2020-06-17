#include "GameManager.h"
#include <log.h>
#include <System.h>
#include <stdlib.h>
#include <stdio.h>

static GameState gameState;

void LT_GameStateInit(const char* in_gameName) {
    gameState.callerGame  = NULL;
    gameState.customData  = NULL;
    gameState.currentGame = NULL;

    SharedLib lib = LT_LoadSharedLibrary("game");
    if (!lib) {
        log_info("Couldn't find library %s.dll", in_gameName);
        exit(-1);
    }
    log_info("%s.dll open!", in_gameName);
}

void LT_GameStateUpdateCurrent(const double deltaTime) {

}