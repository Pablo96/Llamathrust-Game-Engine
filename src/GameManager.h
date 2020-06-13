#include "Common.h"
#include "Game.h"

typedef struct _GameState {
    const char* callerGame;
    void* customData;
    Game* currentGame;
} GameState;

/**
 * Initialize the game state with:
 * - callerGame to null.
 * - customData to null.
 * - currentGame to the game from the config file.
 * 
 * @Param in_gameName: Name of the shared library
 *      containing the initializer game.
 * */
void LT_GameStateInit(const char* in_gameName);

/**
 * Update the current game.
 * @Param deltaTime: Elapsed time from the last frame in ms.
 * */
void LT_GameStateUpdateCurrent(const double deltaTime);
