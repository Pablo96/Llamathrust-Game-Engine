/**
 * @file Engine.h
 * @author Pablo Narvaja
 * @brief In here you'll find the Engine related code
 * for systems initialization, main loop and systems shutdown.
 **/
#pragma once
#include "ArgsParsing.hpp"
#include <Common.hpp>

namespace LT {
	extern void CloseWindow(void);

	/**
	 * @func Engine_Start
	 * @brief Initialize all systems and the game_client/game_server.
	 **/
	void Engine_Start(const ConfigArgs* args);

	/**
	 * @func Engine_Run
	 * @brief Here run the main loop engine related stuff
	 * @param deltaTime:
	 *	@type const double
	 *	@brief time elapsed between frames.
	 **/
	void Engine_Run(const double deltaTime);

	/**
	 * @func Engine_Shutdown
	 * @brief Memory cleaning and resources deattachment.
	 **/
	void Engine_Shutdown(void);
}
