#pragma once
#include <esp_err.h>

#include <string>

typedef void* httpd_handle_t;

namespace PuaraImpl {

httpd_handle_t start_webserver(void);
void stop_webserver(void);

}  // namespace PuaraImpl