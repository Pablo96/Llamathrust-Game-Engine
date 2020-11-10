/**
 * @file EngineConfig.h
 * @author Pablo Narvaja
 * @brief Here you'll find the configuration struct for storing config data.
 **/
#pragma once
#include "Common.hpp"
#include <memory>

namespace LT {
	// forward declarations
	enum class SERVER_TYPE;
	enum class GRAPHIC_API : uint8;

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
	struct Config {
		GRAPHIC_API graphic_api;
		bool networking_support;
		bool is_server;
		uint16 port;
		char ip[16];

		Config(const char* in_ip, const uint64 in_ipSize) {
			memcpy_s(ip, 16, in_ip, in_ipSize);
		}
	};
}
