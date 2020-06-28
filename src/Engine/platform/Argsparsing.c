#include "ArgsParsing.h"
#include <string.h>

Config parseArgs(const char** argsv, const int size) {
    Config config = {0};

    bool reading_server = LT_FALSE;
    for (uint32 i=0; i < size; i++) {
        const char* string = argsv[i];

        if (strcmp(&string[1], "server") == 0) {
            reading_server = LT_TRUE;
        } else if (reading_server == LT_TRUE) {
            reading_server = LT_FALSE;
            config.isServer = (string, "1") == 0;
        }
    }

    return config;
}