#include "ArgsParsing.hpp"
#include <string>

namespace LT {
    const ConfigArgs* parseArgs(const char** argsv, const int32 size) {
        ConfigArgs* config = new ConfigArgs();

        for (int32 i = 0; i < size; i++) {
            std::string string = argsv[i];
            if (string == "server") {
                config->isServer = true;
            }
        }

        return config;
    }
}
