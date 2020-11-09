#include "Input.hpp"
#include <Common.hpp>
#include <ErrorCodes.hpp>
#include <Platform.hpp>
#include <log.hpp>
#include <stdlib.h>

namespace LT {
    static int32 key_states[(uint64) INPUT_KEY::KEYS_COUNT];

    void InputInit() {
        log_trace("Initializing Input...");
        Platform::InitInput(key_states);
        log_info("Input Initialized...");
    }

    INPUT_STATE GetKeyState(const INPUT_KEY in_key) {
#ifdef LT_DEBUG
        if (in_key >= INPUT_KEY::KEYS_COUNT) {
            log_fatal("Key %u does not exists. Max key value is %u", in_key,
                INPUT_KEY::KEYS_COUNT);
            exit(ERROR_INPUT_KEY_INVALID_VALUE);
        }
#endif
        return (INPUT_STATE)Platform::GetKeyState(key_states[in_key]);
    }
}
