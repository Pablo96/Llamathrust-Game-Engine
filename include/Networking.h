#include "Common.h"
#include "EngineConfig.h"


#define MAX_PORT 65535
#define MAX_CHAR_IP 16

/**
 * @enum SERVER_TYPE
 * @brief Represent the types of server connections.
 **/
typedef enum _ServerTypeEnum {
    UDP,
    TCP
} SERVER_TYPE;

extern void LT_NetworkingInit(const Config* config);