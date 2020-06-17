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

    Game* initGame = GetInitGame();
    if (!initGame) {
        log_info("Couldn't load init game!");
        exit(-1);
    }

    LT_GameStateLoadGame(initGame, 0);
}

void LT_GameStateUpdateCurrent(const double deltaTime) {
    Game* game = gameState.currentGame;
    game->OnUpdate(game, deltaTime);
}

void LT_GameStateLoadGame(Game* in_game, void* in_customData) {
    // If there is a current game set it as the caller game,
    // Call OnStop and OnDestroy
    // Then free its memory
    if (gameState.currentGame) {
        Game* game = gameState.currentGame;
        gameState.callerGame = game->idName;
      
        game->OnStop(game);
        game->OnDestroy(game);
      
        free(gameState.currentGame);
    }
    // Set current game as the loaded game
    gameState.currentGame = in_game;
    // Call OnCreate and OnResume
    gameState.currentGame->OnCreate(in_game, gameState.callerGame, gameState.customData);
    gameState.currentGame->OnResume(in_game);
    
    log_info("Loaded game: %s", gameState.currentGame->idName);
}