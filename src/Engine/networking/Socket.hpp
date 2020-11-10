#pragma once
#include <Common.hpp>

#define MAX_PORT 65535
#define MAX_CHAR_IP 16

namespace LT {
    /**
     * @enum SOCKET_TYPE
     * @brief Represent the types of sokcet communication.
     **/
    enum class PROTOCOL { PROT_UDP, PROT_TCP };

    enum class ADDRESS_VERSION { ADDR_IPV4, ADDR_IPV6 };

    enum class ADDRESS_TYPE { ADDR_NUMERIC, ADDR_DOMAIN };

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
    struct NetAddress {
        void* reserved;
        const char* ip;
        const ADDRESS_VERSION version;
        const ADDRESS_TYPE type;
        const PROTOCOL protocol;
        const uint16 port;
        bool willBind;
        bool isValid;

        NetAddress(const char* ip,
            const uint16 port,
            const ADDRESS_VERSION version,
            const ADDRESS_TYPE type,
            const PROTOCOL protocol,
            const bool will_bind);
        ~NetAddress();
    };

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
    struct NetSocket {
        void* reserved;
        const NetAddress* address;
        bool isValid;
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
        NetSocket(const NetAddress* address);

        NetSocket(const NetAddress* address, void* reserved);

        bool Bind();

        bool Listen();

        NetSocket* Accept();
      
        bool Connect();
        
        bool CloseConnection();
        
        bool Read(byte* stream, uint32* streamSize);
        
        bool Write(const byte* stream, const uint32 streamSize);

        void CloseAndDestroy();
    };
}
