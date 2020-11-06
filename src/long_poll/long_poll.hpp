#pragma once

#include "../cmd_handler/cmd_handler.hpp"

using namespace cURL;
using bot::Vk_cmd_handler;
using bot::Cmds;
using std::to_string;
using std::vector;
using nlohmann::json;

class Lp {
private:
  void         _loop         ();
  void         _get_lp_server();
  void         _errors_handle(long error_code);
  std::string  _server;
  std::string  _key;
  std::string  _ts;
  Logger       _logger{logfile, errfile};
  Cmds         _handler;

public:
  void init_bot();
};
