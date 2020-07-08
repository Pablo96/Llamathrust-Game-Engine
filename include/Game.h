/**
 * @file Game.h
 * @author Pablo Narvaja
 * @brief Contains defines for easier mehtod override and the game structur
 **/

#pragma once
#include "Common.h"

typedef struct _Game Game;

/**
 * @def MAKE_ON_CREATE
 * @param GameStructName
 * @brief Generates a function OnCreate for the GameStructName
 * @example MAKE_ON_CREATE(Test) => void TestOnCreate(params...)
 **/
#define MAKE_ON_CREATE(GameStructName) void MAKE_FN_NAME(GameStructName, OnCreate)(Game* this, const char* callerGame, const void* stateData)
/**
 * @def MAKE_ON_RESUME
 * @param GameStructName
 * @brief Generates a function OnResume for the GameStructName
 * @example MAKE_ON_RESUME(Test) => void TestOnResume(params...)
 **/
#define MAKE_ON_RESUME(GameStructName) void MAKE_FN_NAME(GameStructName, OnResume)(Game* this)
/**
 * @def MAKE_ON_UPDATE
 * @param GameStructName
 * @brief Generates a function OnUpdate for the GameStructName
 * @example MAKE_ON_UPDATE(Test) => void TestOnUpdate(params...)
 **/
#define MAKE_ON_UPDATE(GameStructName) void MAKE_FN_NAME(GameStructName, OnUpdate)(Game* this, const double deltaTime)
/**
 * @def MAKE_ON_STOP
 * @param GameStructName
 * @brief Generates a function OnStop for the GameStructName
 * @example MAKE_ON_STOP(Test) => void TestOnStop(params...)
 **/
#define MAKE_ON_STOP(GameStructName) void MAKE_FN_NAME(GameStructName, OnStop)(Game* this)
/**
 * @def MAKE_ON_DESTROY
 * @param GameStructName
 * @brief Generates a function OnDestroy for the GameStructName
 * @example MAKE_ON_DESTROY(Test) => void TestOnDestroy(params...)
 **/
#define MAKE_ON_DESTROY(GameStructName) void MAKE_FN_NAME(GameStructName, OnDestroy)(Game* this)

/**
 * @def MAKE_CONSTRUCTOR
 * @param GameStructName
 * @brief Generates a function declaration with the naming convection for constructors
 * @example MAKE_CONSTRUCTOR(Test) Game* TestConstructor(Game* gameObject)
 **/
#define MAKE_CONSTRUCTOR(GameStructName) Game* MAKE_FN_NAME(GameStructName, Constructor)(Game* gameObject)

/**
 * @func OnCreateFunc
 * @brief Called the first time the game is loaded.
 * @note In this function you should do initialization of the game.
 * @param this
 *   @type Game pointer
 *   @brief Reference to the game object it works on
 * @param callerGame
 *   @type const char pointer
 *   @brief Name of the caller game
 *   @note This param is null if this is the first game loaded
 * @param stateData
 *   @type void pointer
 *   @brief Data passed by the caller game.
 *   @note This param is null if this is the first game loaded
 **/
typedef void (*OnCreateFunc)(Game* this, const char* callerGame, const void* stateData);

/**
 * @func OnResumeFunc
 * @brief Called every time the game is brought into the screen.
 * @note In this function you might resume the rendering and/or the game.
 * @param this
 *   @type Game pointer
 *   @brief Reference to the game object it works on
 **/
typedef void (*OnResumeFunc)(Game* this);

/**
 * @func OnUpdateFunc
 * @brief Called every frame.
 * @note In this function you should update the logic and rendering of the game.
 * @param this
 *   @type Game pointer
 *   @brief Reference to the game object it works on.
 * @param deltaTime
 *   @type const double
 *   @brief The elapsed time between frames.
 **/
typedef void (*OnUpdateFunc)(Game* this, const double deltaTime);

/**
 * @func OnStopFunc
 * @brief Called every time the game is taken off the screen.
 * @note In this function you might pause the rendering and/or the game.
 * @param this
 *   @type Game pointer
 *   @brief Reference to the game object it works on
 **/
typedef void (*OnStopFunc)(Game* this);
/**
 * @func OnDestroyFunc
 * @brief Called when the game is destroyed.
 * @note In this function you should do memory freeing of the game.
 * @param this
 *   @type Game pointer
 *   @brief Reference to the game object it works on
 **/
typedef void (*OnDestroyFunc)(Game* this);

/**
 * @struct Game
 * @brief Defines a game.
 * 
 * @field idName
 *   @type const char pointer
 *   @brief is the id_name of the game.
 *   @note shuld never be null and most be unique on the entire application
 * @field customData
 *   @type void pointer
 *   @brief custom data the game need between stages
 * @field OnCreate
 *   @type function pointer
 *   @see @func OnCreateFunc
 * @field OnResume
 *   @type function pointer
 *   @see @func OnResumeFunc
 * @field OnUpdate
 *   @type function pointer
 *   @see @func OnUpdateFunc
 * @field OnStop
 *   @type function pointer
 *   @see @func OnStopFunc
 * @field OnDestroy
 *   @type function pointer
 *   @see @func OnDestroyFunc
 **/
typedef struct _Game {
    const char* idName;
    void* customData;
    OnCreateFunc OnCreate;
    OnResumeFunc OnResume;
    OnUpdateFunc OnUpdate;
    OnStopFunc OnStop;
    OnDestroyFunc OnDestroy;
} Game;
