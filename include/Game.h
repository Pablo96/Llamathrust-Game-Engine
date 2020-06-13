#include "Common.h"

typedef struct _Game {
    const char* idName;
    void (*OnCreate)(struct _Game* this, const char* callerGame, const void* stateData);
    void (*OnResume)(struct _Game* this);
    void (*OnUpdate)(struct _Game* this, const double deltaTime);
    void (*OnStop)(struct _Game* this);
    void (*OnDestroy)(struct _Game* this, void* stateData);
} Game;
