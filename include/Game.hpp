/**
 * @file Game.h
 * @author Pablo Narvaja
 * @brief Contains defines for easier mehtod override and the game structur
 **/

#pragma once
#include "Common.hpp"

namespace LT {
    /**
     * @struct Game
     * @brief Defines a game.
     *
     * @field idName
     *   @type const char pointer
     *   @brief is the id_name of the game.
     *   @note shuld never be null and most be unique on the entire application
     *   @note this class take ownership
     * @field customData
     *   @type void pointer
     *   @brief custom data the game need between stages
     * @field OnCreate
     *   @type function pointer
     *   @see @func OnCreateFunc
     * @field OnResume
     *   @type function pointer
     *   @see @func OnResumeFunc
     * @field OnUpdate
     *   @type function pointer
     *   @see @func OnUpdateFunc
     * @field OnStop
     *   @type function pointer
     *   @see @func OnStopFunc
     * @field OnDestroy
     *   @type function pointer
     *   @see @func OnDestroyFunc
     **/
    struct Game {
        const char* idName;
        void* customData;

        Game(const char* name, void* data)
            :idName(name), customData(data) {}

        virtual ~Game() {
            delete[] idName;
        }
        /**
         * @func OnCreate
         * @brief Called the first time the game is loaded.
         * @note In this function you should do initialization of the game.
         * @param callerGame
         *   @type const char pointer
         *   @brief Name of the caller game
         *   @note This param is null if this is the first game loaded
         * @param stateData
         *   @type void pointer
         *   @brief Data passed by the caller game.
         *   @note This param is null if this is the first game loaded
         **/
        virtual void OnCreate(const char* callerGame, const void* stateData) = 0;

        /**
         * @func OnResume
         * @brief Called every time the game is brought into the screen.
         * @note In this function you might resume the rendering and/or the game.
         **/
        virtual void OnResume() = 0;

        /**
         * @func OnUpdate
         * @brief Called every frame.
         * @note In this function you should update the logic and rendering of the game.
         * @param deltaTime
         *   @type const double
         *   @brief The elapsed time between frames.
         **/
        virtual void OnUpdate(const double deltaTime) = 0;

        /**
         * @func OnStop
         * @brief Called every time the game is taken off the screen.
         * @note In this function you might pause the rendering and/or the game.
         **/
        virtual void OnStop() = 0;
        /**
         * @func OnDestroy
         * @brief Called when the game is destroyed.
         * @note In this function you should do memory freeing of the game instead of using the destructor.
         **/
        virtual void OnDestroy() = 0;
    };
}
