/**
 * @file ArgsParsing.h
 * @author Pablo Narvaja
 * @brief Code needed to parse Config arguments.
 **/
#pragma once
#include <Common.h>

/**
 * @struct Config
 * @brief has the engine configuration data.
 * @field isServer:
 *	@type bool
 *	@brief If this is a server application for a game.
 **/
typedef struct _Config {
    bool isServer;
} Config;

/**
 * @func parseArgs
 * @brief parse the executable args.
 * @param argsv:
 *	@type char double pointer
 *	@brief vector of strings
 * @param size:
 *	@type const int
 *	@brief strings count
 * @return Config
 *  @brief configuration struct
 **/
extern Config parseArgs(const char** argsv, const int size);
