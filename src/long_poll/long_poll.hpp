#pragma once

#include "../cmd_handler/cmd_handler.hpp"

class Lp {
private:
  void        loop         ();
  void        get_lp_server();
  void        errors_handle(long error_code);
  std::string server;
  std::string key;
  std::string ts;
  Logger logger{logfile, errfile};
  handler::Cmd_handler handler;

public:
  void init_bot();
};
