#include "Socket.h"
#include <Platform.h>
#include <string.h>

NetSocket *LT_SocketCreateAndOpen(NetSocket *socket, SOCKET_TYPE type,
                                  const char *ip, const uint16 port) {
  if (socket == NULL) {
    socket = malloc(sizeof(Socket));
    memset(socket, 0, sizeof(Socket));
  }
  Socket tmp = {.type = type, .ip = ip, .port = port};

  memcpy(socket, &tmp, sizeof(Socket));

  PlatformSocketCreate(socket, LT_FALSE);
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

void LT_SocketCloseAndDestroy(const NetSocket *socket) {
  PlatformSocketClose(socket);
  socket->isValid = LT_FALSE;
}
