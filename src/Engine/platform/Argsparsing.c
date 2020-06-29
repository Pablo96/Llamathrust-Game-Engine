#include "ArgsParsing.h"
#include <string.h>

Config parseArgs(const char** argsv, const int32 size) {
    Config config = {0};

    for (int32 i=0; i < size; i++) {
        const char* string = argsv[i];
        if (strcmp(string, "server") == 0) {
            config.isServer = LT_TRUE;
        }
    }

    return config;
}
