#include "Socket.hpp"
#include <Platform.hpp>
#include <string>

namespace LT {
    NetAddress::NetAddress(const char* in_ip,
        const uint16 in_port,
        const ADDRESS_VERSION in_version,
        const ADDRESS_TYPE in_type,
        const PROTOCOL in_protocol,
        const bool in_will_bind)
        : ip(in_ip), port(in_port), version(in_version), type(in_type),
        protocol(in_protocol), willBind(in_will_bind) {
        Platform::NetAddressCreate(this);
    }

    NetAddress::~NetAddress() {
        Platform::NetAddressDestroy(this);
    }

    NetSocket::NetSocket(const NetAddress* address) : address(address) {
        Platform::SocketCreate(this);
    }

    NetSocket::NetSocket(const NetAddress* address, void* in_reserved)
        : address(address), reserved(in_reserved) {
        Platform::SocketCreate(this);
    }

    bool NetSocket::Bind() {
        return Platform::SocketBind(this);
    }

    bool NetSocket::Listen() {
        return Platform::SocketListen(this);
    }

    NetSocket* NetSocket::Accept() {
        return Platform::SocketAccept(this);
    }

    bool NetSocket::Connect() {
        return Platform::SocketConnect(this, address);
    }

    bool NetSocket::CloseConnection() {
        return Platform::SocketConnClose(this);
    }

    bool NetSocket::Read(byte* buffer, uint32 bufferSize, uint32* streamSize) {
        return Platform::SocketRecieve(this, reinterpret_cast<char*>(buffer), bufferSize, streamSize);
    }

    bool NetSocket::Write(const byte* stream, const uint32 streamSize) {
        return Platform::SocketSend(this, reinterpret_cast<const char*>(stream), streamSize);
    }

    void NetSocket::CloseAndDestroy() {
        Platform::SocketClose(this);
        isValid = false;
    }
}
