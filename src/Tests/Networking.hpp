#pragma once
#include "../Engine/networking/Socket.h"
#include "test_framework.h"
#include <Common.h>
#include <Thread.h>

void ServerThread(void *this) {
  // start server
  NetAddress *address = malloc(sizeof(NetAddress));
  LT_NetAddressCreate(address, "127.0.0.1", 44754, ADDR_IPV4, ADDR_NUMERIC, PROT_UDP, LT_TRUE);
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
      NULL, "127.0.0.1", 44754, ADDR_IPV4, ADDR_DOMAIN, PROT_UDP, LT_FALSE);
  NetAddress *address = LT_NetAddressCreate(NULL, "127.0.0.1", 44755, ADDR_IPV4,
                                            ADDR_NUMERIC, PROT_UDP, LT_FALSE);
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
  Thread *serverThread = malloc(sizeof(Thread));
  LT_Thread_Create(serverThread, ServerThread, NULL, "Server Thread", NULL);

  Thread *clientThread = malloc(sizeof(Thread));
  LT_Thread_Create(clientThread, ClientThread, NULL, "Client Thread", NULL);

  LT_Thread_Start(serverThread);

  Thread *this = LT_Thread_GetCurrent(NULL);
  LT_Thread_Sleep(this, SECONDS(1));

  LT_Thread_Start(clientThread);

  LT_Thread_Join(serverThread);
  LT_Thread_Join(clientThread);

  LT_Thread_ExitCode(serverThread);
  LT_Thread_ExitCode(clientThread);

  int32 exitCodeServer = serverThread->exitCode;
  int32 exitCodeClient = clientThread->exitCode;

  LT_Thread_Destroy(serverThread);
  LT_Thread_Destroy(clientThread);

  free(serverThread);
  free(clientThread);

  return TEST_ASSERT(exitCodeClient == 0 && exitCodeServer == 0);
END_TEST
