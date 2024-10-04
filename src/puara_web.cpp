#include "puara.h"

#include <esp_http_server.h>

httpd_uri_t Puara::reboot;
httpd_uri_t Puara::index;
httpd_uri_t Puara::style;
// httpd_uri_t Puara::factory;
httpd_uri_t Puara::scan;
// httpd_uri_t Puara::update;
httpd_uri_t Puara::indexpost;
httpd_uri_t Puara::settings;
httpd_uri_t Puara::settingspost;

std::string Puara::APpasswdVal1;
std::string Puara::APpasswdVal2;

httpd_handle_t Puara::webserver;
httpd_config_t Puara::webserver_config;

std::unordered_map<std::string, int> Puara::config_fields = {{"SSID", 1},
                                                             {"APpasswd", 2},
                                                             {"APpasswdValidate", 3},
                                                             {"oscIP1", 4},
                                                             {"oscPORT1", 5},
                                                             {"oscIP2", 6},
                                                             {"oscPORT2", 7},
                                                             {"password", 8},
                                                             {"reboot", 9},
                                                             {"persistentAP", 10},
                                                             {"localPORT", 11}};


httpd_handle_t Puara::start_webserver(void) {
  if (!ApStarted) {
    std::cout << "start_webserver: Cannot start webserver: AP and STA not initializated"
              << std::endl;
    return NULL;
  }
  Puara::webserver = NULL;

  Puara::webserver_config.task_priority = tskIDLE_PRIORITY + 5;
  Puara::webserver_config.stack_size = 4096;
  Puara::webserver_config.core_id = tskNO_AFFINITY;
  Puara::webserver_config.server_port = 80;
  Puara::webserver_config.ctrl_port = 32768;
  Puara::webserver_config.max_open_sockets = 7;
  Puara::webserver_config.max_uri_handlers = 9;
  Puara::webserver_config.max_resp_headers = 9;
  Puara::webserver_config.backlog_conn = 5;
  Puara::webserver_config.lru_purge_enable = true;
  Puara::webserver_config.recv_wait_timeout = 5;
  Puara::webserver_config.send_wait_timeout = 5;
  Puara::webserver_config.global_user_ctx = NULL;
  Puara::webserver_config.global_user_ctx_free_fn = NULL;
  Puara::webserver_config.global_transport_ctx = NULL;
  Puara::webserver_config.global_transport_ctx_free_fn = NULL;
  Puara::webserver_config.open_fn = NULL;
  Puara::webserver_config.close_fn = NULL;
  Puara::webserver_config.uri_match_fn = NULL;

  Puara::index.uri = "/";
  Puara::index.method = HTTP_GET, Puara::index.handler = index_get_handler,
  Puara::index.user_ctx = (char*)"/spiffs/index.html";

  Puara::indexpost.uri = "/";
  Puara::indexpost.method = HTTP_POST, Puara::indexpost.handler = index_post_handler,
  Puara::indexpost.user_ctx = (char*)"/spiffs/index.html";

  Puara::style.uri = "/style.css";
  Puara::style.method = HTTP_GET, Puara::style.handler = style_get_handler,
  Puara::style.user_ctx = (char*)"/spiffs/style.css";

  // Puara::factory.uri = "/factory.html";
  // Puara::factory.method    = HTTP_GET,
  // Puara::factory.handler   = get_handler,
  // Puara::factory.user_ctx  = (char*)"/spiffs/factory.html";

  Puara::reboot.uri = "/reboot.html";
  Puara::reboot.method = HTTP_GET, Puara::reboot.handler = get_handler,
  Puara::reboot.user_ctx = (char*)"/spiffs/reboot.html";

  Puara::scan.uri = "/scan.html";
  Puara::scan.method = HTTP_GET, Puara::scan.handler = scan_get_handler,
  Puara::scan.user_ctx = (char*)"/spiffs/scan.html";

  // Puara::update.uri = "/update.html";
  // Puara::update.method    = HTTP_GET,
  // Puara::update.handler   = get_handler,
  // Puara::update.user_ctx  = (char*)"/spiffs/update.html";

  Puara::settings.uri = "/settings.html";
  Puara::settings.method = HTTP_GET, Puara::settings.handler = settings_get_handler,
  Puara::settings.user_ctx = (char*)"/spiffs/settings.html";

  Puara::settingspost.uri = "/settings.html";
  Puara::settingspost.method = HTTP_POST, Puara::settingspost.handler = settings_post_handler,
  Puara::settingspost.user_ctx = (char*)"/spiffs/settings.html";

  // Start the httpd server
  std::cout << "webserver: Starting server on port: " << webserver_config.server_port << std::endl;
  if (httpd_start(&webserver, &webserver_config) == ESP_OK) {
    // Set URI handlers
    std::cout << "webserver: Registering URI handlers" << std::endl;
    httpd_register_uri_handler(webserver, &index);
    httpd_register_uri_handler(webserver, &indexpost);
    httpd_register_uri_handler(webserver, &style);
    httpd_register_uri_handler(webserver, &scan);
    // httpd_register_uri_handler(webserver, &factory);
    httpd_register_uri_handler(webserver, &reboot);
    // httpd_register_uri_handler(webserver, &update);
    httpd_register_uri_handler(webserver, &settings);
    httpd_register_uri_handler(webserver, &settingspost);
    return webserver;
  }

  std::cout << "webserver: Error starting server!" << std::endl;
  return NULL;
}

void Puara::stop_webserver(void) {
  // Stop the httpd server
  httpd_stop(webserver);
}


std::string Puara::prepare_index() {
  Puara::mount_spiffs();
  std::cout << "http (spiffs): Reading index file" << std::endl;
  std::ifstream in("/spiffs/index.html");
  std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
  // Put the module info on the HTML before send response
  Puara::find_and_replace("%DMINAME%", Puara::dmiName, contents);
  if (Puara::StaIsConnected) {
    Puara::find_and_replace("%STATUS%",
                            "Currently connected on "
                            "<strong style=\"color:Tomato;\">" +
                                Puara::wifiSSID + "</strong> network",
                            contents);
  } else {
    Puara::find_and_replace("%STATUS%", "Currently not connected to any network", contents);
  }
  Puara::find_and_replace("%CURRENTSSID%", Puara::currentSSID, contents);
  Puara::find_and_replace("%CURRENTPSK%", Puara::wifiPSK, contents);
  Puara::checkmark("%CURRENTPERSISTENT%", Puara::persistentAP, contents);
  Puara::find_and_replace("%DEVICENAME%", Puara::device, contents);
  Puara::find_and_replace("%CURRENTOSC1%", Puara::oscIP1, contents);
  Puara::find_and_replace("%CURRENTPORT1%", Puara::oscPORT1, contents);
  Puara::find_and_replace("%CURRENTOSC2%", Puara::oscIP2, contents);
  Puara::find_and_replace("%CURRENTPORT2%", Puara::oscPORT2, contents);
  Puara::find_and_replace("%CURRENTLOCALPORT%", Puara::localPORT, contents);
  Puara::find_and_replace("%CURRENTSSID2%", Puara::wifiSSID, contents);
  Puara::find_and_replace("%CURRENTIP%", Puara::currentSTA_IP, contents);
  Puara::find_and_replace("%CURRENTAPIP%", Puara::currentAP_IP, contents);
  Puara::find_and_replace("%CURRENTSTAMAC%", Puara::currentSTA_MAC, contents);
  Puara::find_and_replace("%CURRENTAPMAC%", Puara::currentAP_MAC, contents);
  std::ostringstream tempBuf;
  tempBuf << std::setfill('0') << std::setw(3) << std::hex << Puara::id;
  Puara::find_and_replace("%MODULEID%", tempBuf.str(), contents);
  Puara::find_and_replace("%MODULEAUTH%", Puara::author, contents);
  Puara::find_and_replace("%MODULEINST%", Puara::institution, contents);
  Puara::find_and_replace("%MODULEVER%", Puara::version, contents);

  Puara::unmount_spiffs();

  return contents;
}

esp_err_t Puara::index_get_handler(httpd_req_t* req) {
  std::string prepared_index = prepare_index();
  httpd_resp_sendstr(req, prepared_index.c_str());

  return ESP_OK;
}

esp_err_t Puara::settings_get_handler(httpd_req_t* req) {
  Puara::mount_spiffs();
  std::cout << "http (spiffs): Reading settings file" << std::endl;
  std::ifstream in("/spiffs/settings.html");
  std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

  std::cout << "settings_get_handler: Adding variables to HTML" << std::endl;
  std::string settings;
  for (auto it : variables) {
    if (it.type == "text") {
      settings.append(
          "<div class=\"row\"><div class=\"col-25\"><label "
          "for=\"%PARAMETER%\">%PARAMETER%</label></div><div class=\"col-75\"><input type=\"text\" "
          "id=\"%PARAMETER%\" name=\"%PARAMETER%\" value=\"%PARAMETERVALUE%\"></div></div>");
      find_and_replace("%PARAMETERVALUE%", it.textValue, settings);
      find_and_replace("%PARAMETER%", it.name, settings);
    } else if (it.type == "number") {
      settings.append(
          "<div class=\"row\"><div class=\"col-25\"><label "
          "for=\"%PARAMETER%\">%PARAMETER%</label></div><div class=\"col-75\"><input "
          "type=\"number\" step=\"0.000001\" id=\"%PARAMETER%\" name=\"%PARAMETER%\" "
          "value=\"%PARAMETERVALUE%\"></div></div>");
      find_and_replace("%PARAMETERVALUE%", it.numberValue, settings);
      find_and_replace("%PARAMETER%", it.name, settings);
    }
  }
  find_and_replace("%DATAFROMMODULE%", settings, contents);
  httpd_resp_sendstr(req, contents.c_str());

  return ESP_OK;
}

esp_err_t Puara::settings_post_handler(httpd_req_t* req) {
  char buf[200];

  int api_return, remaining = req->content_len;

  while (remaining > 0) {
    /* Read the data for the request */
    if ((api_return = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf)))) <= 0) {
      if (api_return == HTTPD_SOCK_ERR_TIMEOUT) {
        /* Retry receiving if timeout occurred */
        continue;
      }
      return ESP_FAIL;
    }
    std::string str_buf = convertToString(buf);
    std::string str_token;
    std::string field;
    size_t pos = 0;
    size_t field_pos = 0;
    std::string delimiter = "&";
    std::string field_delimiter = "=";
    // adding delimiter to process last variable in the loop
    str_buf.append(delimiter);

    std::cout << "Settings stored:" << std::endl;
    while ((pos = str_buf.find(delimiter)) != std::string::npos) {
      str_token = str_buf.substr(0, pos);
      field_pos = str_buf.find(field_delimiter);
      field = str_token.substr(0, field_pos);
      str_token.erase(0, field_pos + field_delimiter.length());
      std::cout << field << ": ";
      if (variables.at(variables_fields.at(field)).type == "text") {
        variables.at(variables_fields.at(field)).textValue = urlDecode(str_token);
      } else if (variables.at(variables_fields.at(field)).type == "number") {
        variables.at(variables_fields.at(field)).numberValue = std::stod(str_token);
      }
      std::cout << str_token << std::endl;
      str_buf.erase(0, pos + delimiter.length());
    }
    std::cout << std::endl;
    remaining -= api_return;
  }

  write_settings_json();
  mount_spiffs();
  std::cout << "http (spiffs): Reading saved.html file" << std::endl;
  std::ifstream in("/spiffs/saved.html");
  std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
  httpd_resp_sendstr(req, contents.c_str());
  unmount_spiffs();

  return ESP_OK;
}

esp_err_t Puara::get_handler(httpd_req_t* req) {
  const char* resp_str = (const char*)req->user_ctx;
  Puara::mount_spiffs();
  std::cout << "http (spiffs): Reading requested file" << std::endl;
  std::ifstream in(resp_str);
  std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
  httpd_resp_sendstr(req, contents.c_str());

  Puara::unmount_spiffs();

  return ESP_OK;
}

esp_err_t Puara::style_get_handler(httpd_req_t* req) {
  const char* resp_str = (const char*)req->user_ctx;
  Puara::mount_spiffs();
  std::cout << "http (spiffs): Reading style.css file" << std::endl;
  std::ifstream in(resp_str);
  std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
  httpd_resp_set_type(req, "text/css");
  httpd_resp_sendstr(req, contents.c_str());

  Puara::unmount_spiffs();

  return ESP_OK;
}

esp_err_t Puara::scan_get_handler(httpd_req_t* req) {
  const char* resp_str = (const char*)req->user_ctx;
  Puara::mount_spiffs();
  std::cout << "http (spiffs): Reading scan.html file" << std::endl;
  std::ifstream in(resp_str);
  std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
  wifi_scan();
  find_and_replace("%SSIDS%", wifiAvailableSsid, contents);
  httpd_resp_sendstr(req, contents.c_str());

  Puara::unmount_spiffs();

  return ESP_OK;
}

// esp_err_t Puara::update_get_handler(httpd_req_t *req) {

//     const char* resp_str = (const char*) req->user_ctx;
//     Puara::mount_spiffs();
//     std::cout << "http (spiffs): Reading update.html file" << std::endl;
//     std::ifstream in(resp_str);
//     std::string contents((std::istreambuf_iterator<char>(in)),
//     std::istreambuf_iterator<char>());
//     //httpd_resp_set_type(req, "text/html");
//     httpd_resp_sendstr(req, contents.c_str());

//     Puara::unmount_spiffs();

//     return ESP_OK;
// }

esp_err_t Puara::index_post_handler(httpd_req_t* req) {
  char buf[200];
  bool ret_flag = false;

  int api_return, remaining = req->content_len;

  while (remaining > 0) {
    /* Read the data for the request */
    if ((api_return = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf)))) <= 0) {
      if (api_return == HTTPD_SOCK_ERR_TIMEOUT) {
        /* Retry receiving if timeout occurred */
        continue;
      }
      return ESP_FAIL;
    }

    std::string str_buf = convertToString(buf);
    std::string str_token;
    std::string field;
    size_t pos = 0;
    size_t field_pos = 0;
    std::string delimiter = "&";
    std::string field_delimiter = "=";
    // adding delimiter to process last variable in the loop
    str_buf.append(delimiter);
    bool checkbox_persistentAP = false;

    while ((pos = str_buf.find(delimiter)) != std::string::npos) {
      str_token = str_buf.substr(0, pos);
      field_pos = str_buf.find(field_delimiter);
      field = str_token.substr(0, field_pos);
      str_token.erase(0, field_pos + field_delimiter.length());
      if (config_fields.find(field) != config_fields.end()) {
        switch (config_fields.at(field)) {
          case 1:
            std::cout << "SSID: " << str_token << std::endl;
            if (!str_token.empty()) {
              wifiSSID = urlDecode(str_token);
            } else {
              std::cout << "SSID empty! Keeping the stored value" << std::endl;
            }
            break;
          case 2:
            std::cout << "APpasswd: " << str_token << std::endl;
            if (!str_token.empty()) {
              APpasswdVal1 = urlDecode(str_token);
            } else {
              std::cout << "APpasswd empty! Keeping the stored value" << std::endl;
              APpasswdVal1.clear();
            };
            break;
          case 3:
            std::cout << "APpasswdValidate: " << str_token << std::endl;
            if (!str_token.empty()) {
              APpasswdVal2 = urlDecode(str_token);
            } else {
              std::cout << "APpasswdValidate empty! Keeping the stored value" << std::endl;
              APpasswdVal2.clear();
            };
            break;
          case 4:
            std::cout << "oscIP1: " << str_token << std::endl;
            if (!str_token.empty()) {
              oscIP1 = str_token;
            } else {
              std::cout << "oscIP1 empty! Keeping the stored value" << std::endl;
            }
            break;
          case 5:
            std::cout << "oscPORT1: " << str_token << std::endl;
            if (!str_token.empty()) {
              oscPORT1 = stoi(str_token);
            } else {
              std::cout << "oscPORT1 empty! Keeping the stored value" << std::endl;
            }
            break;
          case 6:
            std::cout << "oscIP2: " << str_token << std::endl;
            if (!str_token.empty()) {
              oscIP2 = str_token;
            } else {
              std::cout << "oscIP2 empty! Keeping the stored value" << std::endl;
            }
            break;
          case 7:
            std::cout << "oscPORT2: " << str_token << std::endl;
            if (!str_token.empty()) {
              oscPORT2 = stoi(str_token);
            } else {
              std::cout << "oscPORT2 empty! Keeping the stored value" << std::endl;
            }
            break;
          case 8:
            std::cout << "password: " << str_token << std::endl;
            if (!str_token.empty()) {
              wifiPSK = urlDecode(str_token);
            } else {
              std::cout << "password empty! Keeping the stored value" << std::endl;
            }
            break;
          case 9:
            std::cout << "Rebooting\n";
            ret_flag = true;
            break;
          case 10:
            std::cout << "persistentAP: " << str_token << std::endl;
            checkbox_persistentAP = true;
            break;
          case 11:
            std::cout << "localPORT: " << str_token << std::endl;
            if (!str_token.empty()) {
              localPORT = stoi(str_token);
            } else {
              std::cout << "localPORT empty! Keeping the stored value" << std::endl;
            }
            break;
          default:
            std::cout << "Error, no match for config field to store received data\n";
            break;
        }
      } else {
        std::cout << "Error, no match for config field to store received data: " << field
                  << std::endl;
      }
      str_buf.erase(0, pos + delimiter.length());
    }

    // processing some post info
    if (APpasswdVal1 == APpasswdVal2 && !APpasswdVal1.empty() && APpasswdVal1.length() > 7) {
      APpasswd = APpasswdVal1;
      std::cout << "Puara password changed!\n";
    } else {
      std::cout
          << "Puara password doesn't match or shorter than 8 characteres. Passwork not changed.\n";
    }
    persistentAP = checkbox_persistentAP;
    APpasswdVal1.clear();
    APpasswdVal2.clear();

    remaining -= api_return;
  }

  if (ret_flag) {
    mount_spiffs();
    std::cout << "http (spiffs): Reading reboot.html file" << std::endl;
    std::ifstream in("/spiffs/reboot.html");
    std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    httpd_resp_sendstr(req, contents.c_str());
    unmount_spiffs();
    std::cout << "\nRebooting...\n" << std::endl;
    xTaskCreate(&Puara::reboot_with_delay, "reboot_with_delay", 1024, NULL, 10, NULL);
  } else {
    write_config_json();
    mount_spiffs();
    std::cout << "http (spiffs): Reading saved.html file" << std::endl;
    std::ifstream in("/spiffs/saved.html");
    std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    httpd_resp_sendstr(req, contents.c_str());
    unmount_spiffs();
  }

  return ESP_OK;
}
