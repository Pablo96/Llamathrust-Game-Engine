#include "Socket.h"
#include <Platform.h>
#include <string.h>

NetAddress *LT_NetAddressCreate(NetAddress *in_mem, const char *ip,
                                const uint16 port,
                                const ADDRESS_VERSION version,
                                const ADDRESS_TYPE type,
                                const PROTOCOL protocol,
                                const bool will_bind) {
  if (in_mem == NULL) {
    in_mem = malloc(sizeof(NetAddress));
    memset(in_mem, 0, sizeof(NetAddress));
  }

  NetAddress tmp = {.ip = ip,
                    .port = port,
                    .version = version,
                    .type = type,
                    .protocol = protocol,
                    .willBind = will_bind
  };
  memcpy(in_mem, &tmp, sizeof(NetAddress));

  PlatformNetAddress(in_mem);

  return in_mem;
}

void LT_NetAddressDestroy(NetAddress *address) {
  PlatformNetAddressDestroy(address);
}

NetSocket *LT_SocketCreate(NetSocket *socket, const NetAddress *address) {
  if (socket == NULL) {
    socket = malloc(sizeof(NetSocket));
    memset(socket, 0, sizeof(NetSocket));
  }

  NetSocket tmp = {.address = address};
  memcpy(socket, &tmp, sizeof(NetSocket));

  PlatformSocketCreate(socket);
  return socket;
}

bool LT_SocketBind(const NetSocket *socket) {
  return PlatformSocketBind(socket);
}

bool LT_SocketListen(const NetSocket *socket) {
  return PlatformSocketListen(socket);
}

NetSocket *LT_SocketAccept(const NetSocket *socket) {
  return PlatformSocketAccept(socket);
}

bool LT_SocketConnect(NetSocket *socket, const NetAddress *address) {
  return PlatformSocketConnect(socket, address);
}

bool LT_SocketCloseConnection(const NetSocket *socket) {
  return PlatformSocketConnClose(socket);
}

bool LT_SocketRead(const NetSocket *socket, byte *stream, uint32 *streamSize) {
  return PlatformSocketRecieve(socket, stream, streamSize);
}

bool LT_SocketWrite(const NetSocket *socket, const byte *stream,
                    const uint32 streamSize) {
  return PlatformSocketSend(socket, stream, streamSize);
}

void LT_SocketCloseAndDestroy(NetSocket *socket) {
  PlatformSocketClose(socket);
  socket->isValid = LT_FALSE;
}
