/**
 * @file GameManager.h
 * @author Pablo Narvaja
 * @brief Here is all code related to Game Loading and States between them.
 **/
#pragma once
#include "Common.h"
#include <Game.h>

/**
 * @struct GameState
 * @brief Defines a global state between games.
 * 
 * @field callerGame
 *   @type const char pointer
 *   @brief is the id_name of the game that requested the load of the current game
 *   @note if the game if the first game loaded this field is null
 * @field customData
 *   @type void pointer
 *   @brief data passed from one game to the other
 * @field currentGame
 *   @type Game pointer
 *   @brief Game currently running
 */
typedef struct _GameState {
    const char* callerGame;
    void* customData;
    Game* currentGame;
} GameState;

/**
 * @func LT_GameStateInit 
 * @brief Initialize the game state with:
 *        - callerGame to null.
 *        - customData to null.
 *        - currentGame to the game from the config file.
 * 
 * @param in_gameName
 *   @type const char pointer
 *   @brief Name of the shared library containing the first game.
 * */
void LT_GameStateInit(const char* in_gameName);

/**
 * @func LT_GameStateUpdateCurrent
 * @brief Update the current game.
 * 
 * @param deltaTime
 *   @type double
 *   @brief Elapsed time from the last frame in ms.
 * */
void LT_GameStateUpdateCurrent(const double deltaTime);

/**
 * @func LT_GameStateLoadGame 
 * @brief Set callerGame to currentGame idName and call in_game OnCreate and OnLoad
 * 
 * @param in_game
 *   @type Game pointer
 *   @brief Game object to load
 * @param in_customData
 *   @type void pointer
 *   @brief data being passed to the game to load
 **/
void LT_GameStateLoadGame(Game* in_game, void* in_customData);
