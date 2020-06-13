#include "Common.h"
#include <Game.h>

/**
 * @struct GameState
 * @brief Defines a global state between games.
 */
typedef struct _GameState {
    const char* callerGame; // @var Name of the game before the current one
    void* customData;
    Game* currentGame;
} GameState;

/**
 * @fn LT_GameStateInit 
 * @brief Initialize the game state with:
 *          - callerGame to null.
 *          - customData to null.
 *          - currentGame to the game from the config file.
 * 
 * @param in_gameName Name of the shared library
 *                  containing the initializer game.
 * */
void LT_GameStateInit(const char* in_gameName);

/**
 * Update the current game.
 * @param deltaTime Elapsed time from the last frame in ms.
 * */
void LT_GameStateUpdateCurrent(const double deltaTime);
