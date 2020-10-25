#pragma once

#include "../cmd_handler/cmd_handler.hpp"

class Lp {
private:
  void   get_lp_server();
  void   loop();
  void   errors_handle(long error_code);
  string server;
  string key;
  string ts;

public:
  void init_bot();
};
