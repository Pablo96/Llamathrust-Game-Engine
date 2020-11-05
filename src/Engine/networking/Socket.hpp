#pragma once
#include <Common.hpp>

#define MAX_PORT 65535
#define MAX_CHAR_IP 16

/**
 * @enum SOCKET_TYPE
 * @brief Represent the types of sokcet communication.
 **/
typedef enum _ProtocolEnum { PROT_UDP, PROT_TCP } PROTOCOL;

typedef enum _AddressVersionEnum { ADDR_IPV4, ADDR_IPV6 } ADDRESS_VERSION;

typedef enum _AddressTypeEnum { ADDR_NUMERIC, ADDR_DOMAIN } ADDRESS_TYPE;

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
  void *reserved;
  const char *ip;
  const ADDRESS_VERSION version;
  const ADDRESS_TYPE type;
  const PROTOCOL protocol;
  const uint16 port;
  bool willBind;
  bool isValid;
} NetAddress;

extern NetAddress *LT_NetAddressCreate(NetAddress *in_mem, const char *ip,
                                       const uint16 port,
                                       const ADDRESS_VERSION version,
                                       const ADDRESS_TYPE type,
                                       const PROTOCOL protocol,
                                       const bool will_bind);
extern void LT_NetAddressDestroy(NetAddress *address);

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
 *	@brief false if the socket is invalid because of an error.
 **/
typedef struct _NetSocket {
  void *reserved;
  const NetAddress *address;
  bool isValid;
} NetSocket;

/**
 * @func LT_SocketCreateAndOpen
 * @brief Creates an open socket.
 * @param socket:
 *	@type NetSocket pointer
 *	@brief OPTIONAL: reserved memory for the socket
 * @param address:
 *	@type NetAddress
 *	@brief addres info of the socket to create
 * @return NetSocket pointer
 *  @brief The socket param value if any or the created socket
 **/
extern NetSocket *LT_SocketCreate(NetSocket *socket, const NetAddress *address);
extern bool LT_SocketBind(const NetSocket *socket);
extern bool LT_SocketListen(const NetSocket *socket);
extern NetSocket *LT_SocketAccept(const NetSocket *socket);
extern bool LT_SocketConnect(NetSocket *socket, const NetAddress *address);
extern bool LT_SocketCloseConnection(const NetSocket *socket);
extern bool LT_SocketRead(const NetSocket *socket, byte *stream,
                          uint32 *streamSize);
extern bool LT_SocketWrite(const NetSocket *socket, const byte *stream,
                           const uint32 streamSize);
extern void LT_SocketCloseAndDestroy(NetSocket *socket);
