/**
 * @file EngineConfig.h
 * @author Pablo Narvaja
 * @brief Here you'll find the configuration struct for storing config data.
 **/
#pragma once
#include "Common.h"

// forward declarations
typedef enum _ServerTypeEnum SERVER_TYPE;
typedef enum _APIs API;

/**
 * @struct Config
 * @brief Represents the config file.
 * @field graphic_api:
 *	@type API enum
 *	@brief the graphic api to use.
 *  @note ignored if is_server is true
 * @field networking_support
 *  @type boolean
 *  @brief if networking is enabled
 * @field is_server
 *  @type boolean
 *  @brief if this is an instance server(true) or a client(false)
 * @field port
 *  @type uint16
 *  @brief port of the networking socket
 * @field ip
 *  @type char array [16]
 *  @brief ip of the networking socket
 **/
typedef struct _Config {
  API graphic_api;
  bool networking_support;
  bool is_server;
  uint16 port;
  char ip[16];
} Config;
