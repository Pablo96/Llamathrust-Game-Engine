#include "TestGame.h"
#include <Common.h>
#include <Game.h>
#include <string.h>

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

MAKE_CONSTRUCTOR(Test) {
    gameObject->idName = "Test Game";
    gameObject->OnCreate = TestOnCreate;
    gameObject->OnResume = TestOnResume;
    gameObject->OnUpdate = TestOnUpdate;
    gameObject->OnStop = TestOnStop;
    gameObject->OnDestroy = TestOnDestroy;
    return gameObject;
}
