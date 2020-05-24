#include "Input.h"
#include <Common.h>
#include <log.h>
#include <stdlib.h>
static int32 key_states[KEYS_COUNT];

void LT_InputInit() {
  log_trace("Initializing Input...");
  InitPlatformInput(key_states);
  log_info("Input Initialized...");
}

LT_INPUT_STATE LT_GetKeyState(const LT_INPUT_KEY in_key) {
#ifdef LT_DEBUG
  if (in_key >= KEYS_COUNT) {
    log_fatal("Key %u does not exists. Max key value is %u", in_key, KEYS_COUNT);
    exit(36);
  }
#endif
  log_info("Requesting %u's state...", in_key);
  return (LT_INPUT_STATE) GetPlatformKeyState(key_states[in_key]);
}
