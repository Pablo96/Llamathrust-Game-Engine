/**
 * @file ArgsParsing.h
 * @author Pablo Narvaja
 * @brief Code needed to parse Config arguments.
 **/
#pragma once
#include <Common.hpp>

/**
 * @struct ConfigArgs
 * @brief has the engine configuration data from execution params.
 * @field isServer:
 *	@type bool
 *	@brief If this is a server application for a game.
 **/
struct ConfigArgs {
  bool isServer;
};

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
const ConfigArgs *parseArgs(const char **argsv, const int size);
