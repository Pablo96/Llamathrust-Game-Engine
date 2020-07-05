#include <Networking.h>
#include <log.h>

void LT_NetworkingInit(const Config* config) {
  if (config->networking_support == LT_FALSE)
    return;

  log_info("Networking enabled!");
  log_info("\tIs server: %s", config->is_server == 0 ? "fasle" : "true");
}
