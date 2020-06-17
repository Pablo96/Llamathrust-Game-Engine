#include "GameManager.h"
#include <log.h>
#include <System.h>
#include <stdlib.h>
#include <stdio.h>

static GameState gameState;
typedef Game*(*LT_GetInitGame)(void);

void LT_GameStateInit(const char* in_gameName) {
    gameState.callerGame  = NULL;
    gameState.customData  = NULL;
    gameState.currentGame = NULL;

    SharedLib lib = LT_LoadSharedLibrary("game");
    if (!lib) {
        log_error("Couldn't find library %s.dll", in_gameName);
        exit(-1);
    }
    log_info("%s.dll open!", in_gameName);


    LT_GetInitGame GetInitGame = (LT_GetInitGame)LT_GetProcAddrFromSharedLib(lib, "GetInitGame");
    if (!GetInitGame) {
      log_error("Couldn't load proc \'GetInitGame\'");
      exit(-1);
    }

    gameState.currentGame = GetInitGame();

    if (!gameState.currentGame) {
        log_info("Couldn't load init game!");
        exit(-1);
    }
    log_info("Loaded game: %s", gameState.currentGame->idName);
}

void LT_GameStateUpdateCurrent(const double deltaTime) {

}