#pragma once
#include <Common.hpp>
#include <Thread.hpp>

#include "../Engine/networking/Socket.hpp"
#include "test_framework.hpp"

PROC_RETURN_T ServerThread(void *ignored_thread_ptr) {
  // start server
  LT::NetAddress *address = new LT::NetAddress(
      "127.0.0.1", 44754, LT::ADDRESS_VERSION::ADDR_IPV4,
      LT::ADDRESS_TYPE::ADDR_NUMERIC, LT::PROTOCOL::PROT_UDP, true);
  LT::NetSocket *server = new LT::NetSocket(address);
  server->Bind();

  // listen for connections
  server->Listen();

  // accept client
  LT::NetSocket *client = server->Accept();

  // send msg to client

  // close connection
  server->CloseConnection();

  // close server
  server->CloseAndDestroy();
  delete server;
  delete address;
  server = nullptr;
  address = nullptr;

  LT::Thread::Exit(reinterpret_cast<int64>(TEST_SUCCESS));
}

PROC_RETURN_T ClientThread(void *ignored_thread_ptr) {
  // Get server address
  LT::NetAddress *server_address = new LT::NetAddress(
      "127.0.0.1", 44754, LT::ADDRESS_VERSION::ADDR_IPV4,
      LT::ADDRESS_TYPE::ADDR_DOMAIN, LT::PROTOCOL::PROT_UDP, false);
  LT::NetAddress *address = new LT::NetAddress(
      "127.0.0.1", 44755, LT::ADDRESS_VERSION::ADDR_IPV4,
      LT::ADDRESS_TYPE::ADDR_NUMERIC, LT::PROTOCOL::PROT_UDP, false);
  // Create the client
  LT::NetSocket *client = new LT::NetSocket(address);

  // Connect to the server
  client->Connect();

  // recieve package from server

  // close client
  client->CloseConnection();
  client->CloseAndDestroy();
  delete client;
  delete server_address;
  delete address;
  client = nullptr;
  address = nullptr;
  server_address = nullptr;

  LT::Thread::Exit(reinterpret_cast<int64>(TEST_SUCCESS));
}

START_TEST(TestNetworking)
LT::Thread *serverThread =
    new LT::Thread(ServerThread, NULL, "Server Thread", NULL);

LT::Thread *clientThread =
    new LT::Thread(ClientThread, NULL, "Client Thread", NULL);

serverThread->Start();

LT::Thread *_this = LT::Thread::GetCurrent();
_this->Sleep(LT_SECONDS(1));

_this->Start();

_this->Join(serverThread);
_this->Join(clientThread);

int32 exitCodeServer = serverThread->CaptureExitCode();
int32 exitCodeClient = clientThread->CaptureExitCode();

delete serverThread;
delete clientThread;

serverThread = clientThread = nullptr;

return TEST_ASSERT(exitCodeClient == 0 && exitCodeServer == 0);
END_TEST
