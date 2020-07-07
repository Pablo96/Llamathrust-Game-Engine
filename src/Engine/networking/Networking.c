#include <Networking.h>
#include <log.h>
#include <Platform.h>
#include <string.h>

// This executable socket
const Socket* socket = NULL;

/**
 * @union END_POINT
 * @brief The end point of the socket connection.
 *        Socket/s to which this executable is connected to
 * @field server
 *  @brief Server socket if this executable is a client
 * @field clients
 *  @brief Connected clients if this executable is a server
 **/
static union {
  const Socket* server = NULL;
  const Socket* clients[MAX_CLIENTS];
} END_POINT;
uint8 clientsCount = 0;

void LT_NetworkingInit(const Config* config) {
  if (config->networking_support == LT_FALSE)
    return;

  log_info("Networking enabled!");

  if (strlen(config->ip) > MAX_CHAR_IP) {
    log_error("IP is too long!");
    return;
  }

  Socket socket_tmp = {
    .port = config->port,
    .type = UDP,
    .ip = config->ip
  };

  socket = (Socket*) malloc(sizeof(Socket));
  memcpy(socket, socket_tmp, sizeof(Socket));

  if (config->is_server) {
    log_info("\tIs server!");
    PlatformSocketCreate(socket, LT_TRUE);
    PlatformSocketBind(socket);
  } else {
    log_info("\tIs client!");

  }
}

void LT_NetworkingListen() {
  Socket* clientDescr = PlatformSocketListenAndAccept(socket);
  
  if (clientDescr)
    clients[clientsCount++] = clientDescr;
}

bool LT_NetworkingWrite(const byte* data, const uint32 bytes_count, const uint32 client_index) {
  const Socket* end_socket = clients[client_index];
  return PlatformSocketSend(end_socket, (const char*) data, bytes_count)
}

bool LT_NetworkingRead(byte* data, uint32* bytes_count, const uint32 max_size, const uint32 client_index) {
  const Socket* end_socket = clients[client_index];
  return PlatformSocketRecieve(socket, (char*) data, bytes_count, max_size);
}

void LT_NetworkingShutdown() {
  PlatformSocketConnClose(socket);
  PlatformSocketClose(socket);
}

bool LT_NetworkingConnectionClosed(uint32 bytes_read) {
  return bytes_read == 0;
}
