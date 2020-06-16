#include "Common.h"

#define MAKE_FN_NAME(prefix, subfix) prefix ## subfix

/**
 * @def MAKE_ON_CREATE
 * @param gameStructName
 * @brief Generates a function OnCreate for the gameStructName
 * @example MAKE_ON_CREATE(Test) => void TestOnCreate(params...)
 **/
#define MAKE_ON_CREATE(gameStructName) void MAKE_FN_NAME(gameStructName, OnCreate)(struct _Game* this, const char* callerGame, const void* stateData)
/**
 * @def MAKE_ON_RESUME
 * @param gameStructName
 * @brief Generates a function OnResume for the gameStructName
 * @example MAKE_ON_RESUME(Test) => void TestOnResume(params...)
 **/
#define MAKE_ON_RESUME(gameStructName) void MAKE_FN_NAME(gameStructName, OnResume)(struct _Game* this)
/**
 * @def MAKE_ON_UPDATE
 * @param gameStructName
 * @brief Generates a function OnUpdate for the gameStructName
 * @example MAKE_ON_UPDATE(Test) => void TestOnUpdate(params...)
 **/
#define MAKE_ON_UPDATE(gameStructName) void MAKE_FN_NAME(gameStructName, OnUpdate)(struct _Game* this, const double deltaTime)
/**
 * @def MAKE_ON_STOP
 * @param gameStructName
 * @brief Generates a function OnStop for the gameStructName
 * @example MAKE_ON_STOP(Test) => void TestOnStop(params...)
 **/
#define MAKE_ON_STOP(gameStructName) void MAKE_FN_NAME(gameStructName, OnStop)(struct _Game* this)
/**
 * @def MAKE_ON_DESTROY
 * @param gameStructName
 * @brief Generates a function OnDestroy for the gameStructName
 * @example MAKE_ON_DESTROY(Test) => void TestOnDestroy(params...)
 **/
#define MAKE_ON_DESTROY(gameStructName) void MAKE_FN_NAME(gameStructName, OnDestroy)(struct _Game* this, void* stateData)


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
