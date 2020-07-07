/**
 * @file Networking.h
 * @author Pablo Narvaja
 * @brief All the networking abstraction is found in this file.
 **/
#pragma once
#include "Common.h"
#include "EngineConfig.h"

/**
 * NOTE:
 * Should implment hoffman coding for stream compression
 **/

extern uint8 clientsCount;
#define MAX_PORT 65535
#define MAX_CHAR_IP 16
#define MAX_PACKET_SIZE 512
#define MAX_CLIENTS 64

/**
 * @enum SERVER_TYPE
 * @brief Represent the types of server connections.
 **/
typedef enum _ServerTypeEnum {
    UDP,
    TCP
} SERVER_TYPE;


/**
 * @struct Socket
 * @brief socket abstraction.
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
typedef struct _Socket {
    const char* ip;
    void* reserved;
    SERVER_TYPE type;
    uint16 port;
} Socket;


/**
 * @func LT_NetworkingInit
 * @brief initialize networking if supported.
 * @param config:
 *	@type const Config pointer
 *	@brief Configuration file.
 * @return void
 **/
extern void LT_NetworkingInit(const Config* config);

/**
 * @func LT_NetworkingListen
 * @brief Check for new clients.
 * @param void
 * @return void
 **/
extern void LT_NetworkingListen(void)

/**
 * @func LT_NetworkingWrite
 * @brief send data over the network.
 * @param data:
 *	@type const byte pointer
 *	@brief data to send over.
 * @param bytes_count:
 *	@type uint64
 *	@brief data size in bytes.
 * @param client_index
 *  @type const uint32
 *  @brief client index to which to send
 *  @note If you are the client this should always be 0.
 * @return bool
 *  @brief LT_FALSE if error.
 **/
extern bool LT_NetworkingWrite(const byte* data, const uint32 bytes_count, const uint32 client_index = 0);

/**
 * @func LT_NetworkingWrite
 * @brief send data over the network.
 * @param data:
 *	@type byte pointer
 *	@brief array with the data in read.
 * @param bytes_count:
 *	@type uint64 pointer
 *	@brief data size read in bytes.
 * @param client_index
 *  @type const uint32
 *  @brief client index to which to send
 *  @note If you are the client this should always be 0.
 * @return bool
 *  @brief LT_FALSE if reading problem or connection closed.
 **/
extern bool LT_NetworkingRead(byte* data, uint32* bytes_read, const uint32 max_size, const uint32 client_index = 0);


/**
 * @func LT_NetworkingShutdown
 * @brief Shutdown networking / Close the socket.
 * @return void
 **/
extern void LT_NetworkingShutdown(void);

/**
 * @func LT_NetworkingConnectionClosed
 * @brief Return LT_TRUE if connection closed.
 * @param bytes_read:
 *	@type uint32
 *	@brief bytes read from the socket.
 * @return bool
 **/
extern bool LT_NetworkingConnectionClosed(uint32 bytes_read);
