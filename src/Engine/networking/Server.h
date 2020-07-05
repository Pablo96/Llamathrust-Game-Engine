#include <Common.h>

/**
 * @file Server.h
 * @author Pablo Narvaja
 * @brief In here you'll find the abstraction for server related code.
 **/
#pragma once

/**
 * NOTE:
 * Should implment hoffman coding for stream compression
 **/


/**
 * @struct Server
 * @brief Server abstraction.
 * @field port:
 *	@type int
 *	@brief port that the server listen to.
 * @field ip:
 *	@type char array
 *	@brief ip of the server
 *  @note max length of 16 and can be "localhost"
 * @field type:
 *	@type SERVER_TYPE enum
 *	@brief server type can be TCP or UDP.
 * @field reserved:
 *	@type void pointer
 *	@brief reserved for platform info.
 **/
typedef struct _Server {
    int16 port;
    char[MAX_CHAR_IP] ip;
    SERVER_TYPE type;
    void* reserved;
} Server;


extern const Server* LT_CreateServer(const int16 port, char[MAX_CHAR_IP] ip, const SERVER_TYPE type);

extern void LT_NetSend(const Server* server, const byte msg, const uint64 msg_size);

extern void LT_CloseServer(const Server* server);
