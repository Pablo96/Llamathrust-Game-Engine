#pragma once
#include "Socket.h"
#include <Common.h>
#include <CoreLib/Array.h>
#include <Networking.h>

/**
 * @struct Server
 * @brief Represents a network server
 * @field base
 *  @type const NetSocket
 *  @brief socket of this server
 * @field clients
 *  @type const Socket pointers array
 *  @brief Connected clients
 * @field max_clients
 *  @type const uint8
 *  @brief max clients this server support
 **/
typedef struct _Server {
  const NetSocket base;
  Array clients;
  uint8 max_clients = 0;
} Server;

extern Server *LT_CreateServer(const NetSocket *socket,
                               SocketServerUpdateCallback callback);
extern void LT_ServerUpdate();
extern void LT_ServerShutdown();
