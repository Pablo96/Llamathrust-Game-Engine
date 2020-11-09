#pragma once
#include <Common.hpp>

namespace LT {
	/**
	 * @enum LT_INPUT_KEY
	 * @brief Available keys and buttons.
	 **/
	enum class INPUT_KEY : int32 {
		LT_KEY_MOUSE_BUTTON_L,
		LT_KEY_MOUSE_BUTTON_R,
		LT_KEY_MOUSE_MIDDLE,
		LT_KEY_MOUSE_BUTTON_X1,
		LT_KEY_MOUSE_BUTTON_X2,
		LT_KEY_BACKSPACE,
		LT_KEY_TAB,
		LT_KEY_DEL,
		LT_KEY_ENTER,
		LT_KEY_SHIFT,
		LT_KEY_CONTROL,
		LT_KEY_ALT,
		LT_KEY_MENU,
		LT_KEY_PAUSE,
		LT_KEY_ESCAPE,
		LT_KEY_SPACEBAR,
		LT_KEY_PAGE_UP,
		LT_KEY_PAGE_DOWN,
		LT_KEY_END,
		LT_KEY_HOME,
		LT_KEY_LEFT_ARROW,
		LT_KEY_UP_ARROW,
		LT_KEY_RIGHT_ARROW,
		LT_KEY_DOWN_ARROW,
		LT_KEY_SELECT,
		LT_KEY_SCREENSHOT,
		LT_KEY_EXECUTE,
		LT_KEY_PRINT_SCREEN,
		LT_KEY_INSERT,
		LT_KEY_HELP,
		LT_KEY_NUM_0,
		LT_KEY_NUM_1,
		LT_KEY_NUM_2,
		LT_KEY_NUM_3,
		LT_KEY_NUM_4,
		LT_KEY_NUM_5,
		LT_KEY_NUM_6,
		LT_KEY_NUM_7,
		LT_KEY_NUM_8,
		LT_KEY_NUM_9,
		LT_KEY_A,
		LT_KEY_B,
		LT_KEY_C,
		LT_KEY_D,
		LT_KEY_E,
		LT_KEY_F,
		LT_KEY_G,
		LT_KEY_H,
		LT_KEY_I,
		LT_KEY_J,
		LT_KEY_K,
		LT_KEY_L,
		LT_KEY_M,
		LT_KEY_N,
		LT_KEY_O,
		LT_KEY_P,
		LT_KEY_Q,
		LT_KEY_R,
		LT_KEY_S,
		LT_KEY_T,
		LT_KEY_U,
		LT_KEY_V,
		LT_KEY_W,
		LT_KEY_X,
		LT_KEY_Y,
		LT_KEY_Z,
		LT_KEY_NUMPAD0,
		LT_KEY_NUMPAD1,
		LT_KEY_NUMPAD2,
		LT_KEY_NUMPAD3,
		LT_KEY_NUMPAD4,
		LT_KEY_NUMPAD5,
		LT_KEY_NUMPAD6,
		LT_KEY_NUMPAD7,
		LT_KEY_NUMPAD8,
		LT_KEY_NUMPAD9,
		LT_KEY_MULTIPLY,
		LT_KEY_PLUS,
		LT_KEY_SEPARATOR,
		LT_KEY_SUBTRACT,
		LT_KEY_NUM_DOT,
		LT_KEY_DIVIDE,
		LT_KEY_F1,
		LT_KEY_F2,
		LT_KEY_F3,
		LT_KEY_F4,
		LT_KEY_F5,
		LT_KEY_F6,
		LT_KEY_F7,
		LT_KEY_F8,
		LT_KEY_F9,
		LT_KEY_F10,
		LT_KEY_F11,
		LT_KEY_F12,
		LT_KEY_CAPS_LOCK,
		LT_KEY_NUM_LOCK,
		LT_KEY_SCROLL_LOCK,
		LT_KEY_SUPER_L,
		LT_KEY_SUPER_R,
		LT_KEY_SHIFT_L,
		LT_KEY_SHIFT_R,
		LT_KEY_ALT_L,
		LT_KEY_ALT_R,
		LT_KEY_CONTROL_L,
		LT_KEY_CONTROL_R,
		KEYS_COUNT
	};

	/**
	 * @enum LT_INPUT_STATE
	 * @brief Available keys states.
	 * @tag LT_KEY_UP
	 *	  @brief Key is up (not pressed)
	 * @tag LT_KEY_DOWN
	 *    @brief key is being pressed
	 * @tag LT_KEY_PRESSED
	 *    @brief key was resently pressed
	 *    @note is set when a key resently pass from up to down.
	 **/
	typedef enum class INPUT_STATE {
		LT_KEY_UP,
		LT_KEY_DOWN,
		LT_KEY_PRESSED
	};

	/**
	 * @func LT_InputInit
	 * @brief initialize the input system.
	 * @return void
	 **/
	void InputInit(void);

	/**
	 * @func LT_GetKeyState
	 * @brief Gets the key a state is.
	 * @param in_key
	 *    @type const LT_INPUT_KEY
	 *    @brief the key to what check the state for
	 * @return LT_INPUT_STATE
	 *    @brief the state in what the key is
	 **/
	INPUT_STATE GetKeyState(const INPUT_KEY in_key);
}