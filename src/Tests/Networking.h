#pragma once
#include "../Engine/networking/Socket.h"
#include "test_framework.h"
#include <Common.h>
#include <Thread.h>

void ServerThread(void *this) {
  // start server
  NetAddress *address = malloc(sizeof(NetAddress));
  LT_NetAddressCreate(address, "localhost", 44754, ADDR_IPV4, ADDR_DOMAIN,
                      PROT_UDP);
  NetSocket *server = malloc(sizeof(NetSocket));
  LT_SocketCreate(server, address);
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
  LT_NetAddressDestroy(address);
  free(address);

  LT_Thread_Exit(TEST_SUCCESS);
}

void ClientThread(void *this) {
  // Get server address
  NetAddress *server_address = LT_NetAddressCreate(
      NULL, "127.0.0.1", 44754, ADDR_IPV4, ADDR_DOMAIN, PROT_UDP);
  NetAddress *address = LT_NetAddressCreate(NULL, "localhost", 44755, ADDR_IPV4,
                                            ADDR_DOMAIN, PROT_UDP);
  // Create the client
  NetSocket *client = malloc(sizeof(NetSocket));
  LT_SocketCreate(client, address);

  // Connect to the server
  LT_SocketConnect(client, server_address);

  // recieve package from server

  // close client
  LT_SocketCloseConnection(client);
  LT_SocketCloseAndDestroy(client);

  LT_NetAddressDestroy(address);
  LT_NetAddressDestroy(server_address);
  free(client);
  free(address);
  free(server_address);

  LT_Thread_Exit(TEST_SUCCESS);
}

START_TEST(TestNetworking)

return TEST_SUCCESS;
END_TEST
