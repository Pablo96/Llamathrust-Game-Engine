#include <Common.h>
#include <Game.h>
#include <Platform.h>

MAKE_ON_CREATE(Test) {

}

MAKE_ON_RESUME(Test) {

}

MAKE_ON_UPDATE(Test) {

}

MAKE_ON_STOP(Test) {

}

MAKE_ON_DESTROY(Test) {

}

static Game test = {
    .idName = "Test game",
    .OnCreate = TestOnCreate,
    .OnResume = TestOnResume,
    .OnUpdate = TestOnUpdate,
    .OnStop = TestOnStop,
    .OnDestroy = TestOnDestroy
};

#include <windows.h>
LT_CREATEGAME();
