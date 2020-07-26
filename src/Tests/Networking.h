#pragma once
#include "../Engine/networking/Socket.h"
#include "test_framework.h"
#include <Common.h>
#include <Thread.h>

void ServerThread(void *this) {
  // start server
  NetSocket *server = malloc(sizeof(NetSocket));
  LT_SocketCreateAndOpen(server, NULL, 44754);
  LT_SocketBind(server);

  // listen for connections
  LT_SocketListen(server);

  // accept client
  NetSocket *client = LT_SocketAccept(server);

  // send msg to client

  // close connection
  LT_SocketCloseConnection(server);

  // close server
  LT_SocketCloseAndDestroy(server);

  free(server);

  LT_Thread_Exit(TEST_SUCCESS);
}

void ClientThread(void *this) {
  // recieve package from server
  NetSocket *client = malloc(sizeof(NetSocket));
  LT_SocketCreateAndOpen(client, "127.0.0.1", 44754);

  // close client
  LT_SocketCloseAndDestroy(client);

  LT_Thread_Exit(TEST_SUCCESS);
}

START_TEST(NetworkingTest)

return TEST_ASSERT() END_TEST
