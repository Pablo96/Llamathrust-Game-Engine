#include "Input.h"
#include <Common.h>
#include <log.h>
#include <stdlib.h>
static uint8 key_states[KEYS_COUNT];

LT_INPUT_STATE LT_GetKeyState(const LT_INPUT_KEY in_key) {
#ifdef LT_DEBUG
  if (in_key >= KEYS_COUNT) {
    log_info("Key %u does not exists. Max key value is %u", in_key, KEYS_COUNT);
    exit(36);
  }
#endif
  return (LT_INPUT_STATE) key_states[in_key];
}
