/**
 * @file Networking.h
 * @author Pablo Narvaja
 * @brief All the networking public API is found in this file.
 **/
#pragma once
#include "Common.h"
#include "EngineConfig.h"

/**
 * NOTE:
 * Should implment hoffman coding for stream compression
 **/

#define MAX_PACKET_SIZE 512
#define MAX_CLIENTS 255

/**
 * @func SocketClientUpdateCallback
 * @brief Callback to use the recived stream:
 * @param stream:
 *	@type const byte pointer
 *	@brief recived data.
 * @param streamSize:
 *	@type const uint32
 *	@brief data size.
 * @return void
 **/
typedef void(SocketClientUpdateCallback *)(const byte *stream,
                                           const uint32 streamSize);
/**
 * @func SocketServerUpdateCallback
 * @brief Callback to use the recived stream:
 * @param deltaTime:
 *	@type const double
 *	@brief elapsed time between frames.
 * @param out_stream:
 *	@type byte pointer
 *	@brief data to send.
 * @param out_streamSize:
 *	@type uint32 pointer
 *	@brief data size.
 * @return void
 **/
typedef void(SocketServerUpdateCallback *)(const double deltaTime,
                                           byte *out_stream,
                                           uint32 *out_streamSize);

extern struct _Client;
typedef struct _Client Client;
extern Client *LT_CreateClient(const NetSocket *socket,
                               SocketClientUpdateCallback callback);
