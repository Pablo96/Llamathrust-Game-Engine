#pragma once
#include "Common.h"

#define MAKE_FN_NAME(prefix, subfix) prefix ## subfix

/**
 * @def MAKE_ON_CREATE
 * @param GameStructName
 * @brief Generates a function OnCreate for the GameStructName
 * @example MAKE_ON_CREATE(Test) => void TestOnCreate(params...)
 **/
#define MAKE_ON_CREATE(GameStructName) void MAKE_FN_NAME(GameStructName, OnCreate)(struct _Game* this, const char* callerGame, const void* stateData)
/**
 * @def MAKE_ON_RESUME
 * @param GameStructName
 * @brief Generates a function OnResume for the GameStructName
 * @example MAKE_ON_RESUME(Test) => void TestOnResume(params...)
 **/
#define MAKE_ON_RESUME(GameStructName) void MAKE_FN_NAME(GameStructName, OnResume)(struct _Game* this)
/**
 * @def MAKE_ON_UPDATE
 * @param GameStructName
 * @brief Generates a function OnUpdate for the GameStructName
 * @example MAKE_ON_UPDATE(Test) => void TestOnUpdate(params...)
 **/
#define MAKE_ON_UPDATE(GameStructName) void MAKE_FN_NAME(GameStructName, OnUpdate)(struct _Game* this, const double deltaTime)
/**
 * @def MAKE_ON_STOP
 * @param GameStructName
 * @brief Generates a function OnStop for the GameStructName
 * @example MAKE_ON_STOP(Test) => void TestOnStop(params...)
 **/
#define MAKE_ON_STOP(GameStructName) void MAKE_FN_NAME(GameStructName, OnStop)(struct _Game* this)
/**
 * @def MAKE_ON_DESTROY
 * @param GameStructName
 * @brief Generates a function OnDestroy for the GameStructName
 * @example MAKE_ON_DESTROY(Test) => void TestOnDestroy(params...)
 **/
#define MAKE_ON_DESTROY(GameStructName) void MAKE_FN_NAME(GameStructName, OnDestroy)(struct _Game* this, void* stateData)

/**
 * @def MAKE_CONSTRUCTOR
 * @param GameStructName
 * @brief Generates a function declaration with the naming convection for constructors
 * @example MAKE_CONSTRUCTOR(Test) Game* TestConstructor(Game* gameObject)
 **/
#define MAKE_CONSTRUCTOR(GameStructName) Game* MAKE_FN_NAME(GameStructName, Constructor)(Game* gameObject)

typedef void (*OnCreateFunc)(struct _Game* this, const char* callerGame, const void* stateData);
typedef void (*OnResumeFunc)(struct _Game* this);
typedef void (*OnUpdateFunc)(struct _Game* this, const double deltaTime);
typedef void (*OnStopFunc)(struct _Game* this);
typedef void (*OnDestroyFunc)(struct _Game* this, void* stateData);

typedef struct _Game {
    const char* idName;
    OnCreateFunc OnCreate;
    OnResumeFunc OnResume;
    OnUpdateFunc OnUpdate;
    OnStopFunc OnStop;
    OnDestroyFunc OnDestroy;
} Game;
