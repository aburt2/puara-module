#pragma once
#include <string>
#include <esp_err.h>
namespace Puara {

esp_err_t index_get_handler(httpd_req_t* req);
esp_err_t get_handler(httpd_req_t* req);
esp_err_t style_get_handler(httpd_req_t* req);
esp_err_t settings_get_handler(httpd_req_t* req);
esp_err_t settings_post_handler(httpd_req_t* req);
esp_err_t scan_get_handler(httpd_req_t* req);
esp_err_t index_post_handler(httpd_req_t* req);
std::string prepare_index();

}  // namespace Puara