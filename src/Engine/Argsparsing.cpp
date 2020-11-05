#include "ArgsParsing.hpp"
#include <memory>
#include <string>

const ConfigArgs *parseArgs(const char **argsv, const int32 size) {
  ConfigArgs *config = new ConfigArgs();

  for (int32 i = 0; i < size; i++) {
    const char *string = argsv[i];
    if (strcmp(string, "server") == 0) {
      config->isServer = true;
    }
  }

  return config;
}
