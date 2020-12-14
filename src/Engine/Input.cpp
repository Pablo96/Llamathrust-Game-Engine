#include "Input.hpp"

#include <stdlib.h>

#include <Common.hpp>
#include <ErrorCodes.hpp>
#include <Platform.hpp>
#include <log.hpp>
#include <stdexcept>
#include <string>

namespace LT {
static int32 key_codes[static_cast<uint64>(INPUT_KEY::KEYS_COUNT)];

void InputInit() {
  log_trace("Initializing Input...");
  Platform::InitInput(key_codes);
  log_info("Input Initialized...");
}

INPUT_STATE GetKeyState(const INPUT_KEY in_key) {
#ifdef LT_DEBUG
  if (in_key >= INPUT_KEY::KEYS_COUNT) {
    std::string msg = GET_ERROR_MSG(ERROR_INPUT_KEY_INVALID_VALUE);
    masg += "/nKey %u does not exists. Max key value is %u";
    log_fatal(msg.c_str(), in_key, INPUT_KEY::KEYS_COUNT);
    throw new std::runtime_error(msg);
  }
#endif
  return (INPUT_STATE)Platform::GetKeyState(
      key_codes[*reinterpret_cast<const uint64*>(&in_key)]);
}
}  // namespace LT
