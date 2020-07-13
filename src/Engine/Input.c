#include "Input.h"
#include <Common.h>
#include <Platform.h>
#include <log.h>
#include <stdlib.h>
#include <ErrorCodes.h>
static int32 key_states[KEYS_COUNT];

void LT_InputInit() {
  log_trace("Initializing Input...");
  PlatformInitInput(key_states);
  log_info("Input Initialized...");
}

LT_INPUT_STATE LT_GetKeyState(const LT_INPUT_KEY in_key) {
#ifdef LT_DEBUG
  if (in_key >= KEYS_COUNT) {
    log_fatal("Key %u does not exists. Max key value is %u", in_key,
              KEYS_COUNT);
    exit(ERROR_INPUT_KEY_INVALID_VALUE);
  }
#endif
  return (LT_INPUT_STATE)PlatformGetKeyState(key_states[in_key]);
}
