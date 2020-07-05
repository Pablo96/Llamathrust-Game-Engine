#include "ArgsParsing.h"
#include <string.h>
#include <memory.h>

const ConfigArgs* parseArgs(const char** argsv, const int32 size) {
    ConfigArgs* config = (ConfigArgs*) malloc(sizeof(ConfigArgs));
    memset(config, 0, sizeof(ConfigArgs));

    for (int32 i=0; i < size; i++) {
        const char* string = argsv[i];
        if (strcmp(string, "server") == 0) {
            config.isServer = LT_TRUE;
        }
    }

    return config;
}
