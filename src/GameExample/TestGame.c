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

Game* TestConstructor(Game* testObject) {
    testObject->idName = "Test Game";
    testObject->OnCreate = TestOnCreate;
    testObject->OnResume = TestOnResume;
    testObject->OnUpdate = TestOnUpdate;
    testObject->OnStop = TestOnStop;
    testObject->OnDestroy = TestOnDestroy;
    return testObject;
}
