#include "ArgsParsing.hpp"

#include <string>
#include <vector>

#define LT_ADD_CONFIG_ARG(key, value) configArgsStrings[key] = value
#define LT_GET_CONFIG_STR(key) configArgsStrings[key]

namespace LT {
const ConfigArgs* parseArgs(const char** argsv, const int32 size) {
  std::vector<std::string> configArgsStrings(CONFIG_ARGS::COUNT);
  LT_ADD_CONFIG_ARG(CONFIG_ARGS::SERVER, "server");

  ConfigArgs* config = new ConfigArgs();

  for (int32 i = 0; i < size; i++) {
    std::string string = argsv[i];
    if (string == LT_GET_CONFIG_STR(CONFIG_ARGS::SERVER)) {
      config->isServer = true;
    }
  }

  return config;
}
}  // namespace LT

#undef LT_ADD_CONFIG_ARG
