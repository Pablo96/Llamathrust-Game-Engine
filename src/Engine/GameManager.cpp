#include "GameManager.hpp"
#include <ErrorCodes.hpp>
#include <System.hpp>
#include <log.hpp>
#include <stdio.h>
#include <stdlib.h>

namespace LT {
    static GameState gameState;
    typedef Game* (*LT_GetInitGame)(void);

    void GameStateInit(const char* in_gameName) {
        gameState.callerGame = NULL;
        gameState.customData = NULL;
        gameState.currentGame = NULL;

        SharedLib lib = LT::LoadSharedLibrary("game");
        if (!lib) {
            log_error("Couldn't find library %s.dll", in_gameName);
            exit(ERROR_GAME_LIB_NOT_FOUND);
        }
        log_info("%s.dll open!", in_gameName);

        LT_GetInitGame GetInitGame =
            (LT_GetInitGame)LT::GetProcAddrFromSharedLib(lib, "GetInitGame");
        if (!GetInitGame) {
            log_error("Couldn't load proc \'GetInitGame\'");
            exit(ERROR_GAME_LIB_INVALID);
        }

        Game* initGame = GetInitGame();
        if (!initGame) {
            log_info("Couldn't load init game!");
            exit(ERROR_GAME_MAIN_NOT_FOUND);
        }

        GameStateLoadGame(initGame, 0);
    }

    void GameStateUpdateCurrent(const double deltaTime) {
        Game* game = gameState.currentGame;
        game->OnUpdate(deltaTime);
    }

    void GameStateLoadGame(Game* in_game, void* in_customData) {
        // If there is a current game set it as the caller game,
        // Call OnStop and OnDestroy
        // Then free its memory
        if (gameState.currentGame) {
            Game* game = gameState.currentGame;
            gameState.callerGame = game->idName;

            game->OnStop();
            game->OnDestroy();

            free(gameState.currentGame);
        }
        // Set current game as the loaded game
        gameState.currentGame = in_game;
        // Call OnCreate and OnResume
        gameState.currentGame->OnCreate(gameState.callerGame,
            gameState.customData);
        gameState.currentGame->OnResume();

        log_info("Loaded game: %s", gameState.currentGame->idName);
    }
}
