#include "TestGame.h"
#include <Common.h>
#include <Game.h>
#include <string.h>
#include <stdio.h>

typedef struct _TestCustomData {
    int numberOfUpdates;
    bool isOnScreen;
} TestData;

MAKE_ON_CREATE(Test) {
    TestData* customData = (TestData*) malloc(sizeof(TestData));
    customData->isOnScreen = LT_FALSE;
    customData->numberOfUpdates = 0;
    this->customData = customData;
}

MAKE_ON_RESUME(Test) {
    TestData* customData = (TestData*) this->customData;
    customData->isOnScreen = LT_TRUE;
}

MAKE_ON_UPDATE(Test) {
    TestData* customData = (TestData*) this->customData;
    printf("Number of updates: %u\n", customData->numberOfUpdates++);
}

MAKE_ON_STOP(Test) {
    TestData* customData = (TestData*) this->customData;
    customData->isOnScreen = LT_FALSE;
}

MAKE_ON_DESTROY(Test) {
    free(this->customData);
}

MAKE_CONSTRUCTOR(Test) {
    gameObject->idName = "Test Game";
    gameObject->customData = NULL;
    gameObject->OnCreate = TestOnCreate;
    gameObject->OnResume = TestOnResume;
    gameObject->OnUpdate = TestOnUpdate;
    gameObject->OnStop = TestOnStop;
    gameObject->OnDestroy = TestOnDestroy;
    return gameObject;
}
