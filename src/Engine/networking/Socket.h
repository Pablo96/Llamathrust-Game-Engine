#pragma once
#include <Common.h>

#define MAX_PORT 65535
#define MAX_CHAR_IP 16

/**
 * @enum SOCKET_TYPE
 * @brief Represent the types of sokcet communication.
 **/
typedef enum _SocketTypeEnum { UDP, TCP } SOCKET_TYPE;

/**
 * @struct NetAddress
 * @brief represent a network address.
 * @field ip:
 *	@type char array
 *	@brief ip of the server
 *  @note max length of 16 and can be "localhost"
 * @field port:
 *	@type int
 *	@brief port that the server listen to.
 **/
typedef struct _NetAddress {
  const byte reserved[16];
  const char *ip;
  const uint16 port;
} NetAddress;

/**
 * @struct NetSocket
 * @brief socket abstraction.
 * @field reserved:
 *	@type void pointer
 *	@brief reserved for platform info.
 * @field type:
 *	@type SERVER_TYPE enum
 *	@brief server type can be TCP or UDP.
 * @field isValid:
 *	@type bool
 *	@brief LT_FALSE if the socket is invalid because of an error.
 **/
typedef struct _NetSocket {
  void *reserved;
  NetAddress *address;
  SOCKET_TYPE type;
  bool isValid;
} NetSocket;

/**
 * @func LT_SocketCreateAndOpen
 * @brief Creates an open socket.
 * @param socket:
 *	@type NetSocket pointer
 *	@brief OPTIONAL: reserved memory for the socket
 * @param type:
 *	@type SOCKET_TYPE
 *	@brief type of the socket
 * @param ip:
 *	@type const char pointer
 *	@brief ip of the socket
 *  @note should be in the format of (nnn.nnn.nnn.nnn) if null is a listener
 *socket.
 * @param port:
 *	@type const uint16
 *	@brief port of the socket
 * @return NetSocket pointer
 *  @brief The socket param value if any or the created socket
 **/
extern NetSocket *LT_SocketCreateAndOpen(NetSocket *socket, SOCKET_TYPE type,
                                         const char *ip, const uint16 port);
extern bool LT_SocketBind(const NetSocket *socket);
extern bool LT_SocketListen(const NetSocket *socket);
extern NetSocket *LT_SocketAccept(const NetSocket *socket);
extern bool LT_SocketConnect(const NetSocket *socket);
extern bool LT_SocketCloseConnection(const NetSocket *socket);
extern bool LT_SocketRead(const NetSocket *socket, byte *stream,
                          uint32 *streamSize);
extern bool LT_SocketWrite(const NetSocket *socket, const byte *stream,
                           const uint32 streamSize);
extern void LT_SocketCloseAndDestroy(const NetSocket *socket);
