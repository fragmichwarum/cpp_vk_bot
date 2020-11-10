#pragma once

#include "../cmd_handler/cmd_handler.hpp"

namespace std
  {
    class thread;
  }

using namespace cURL;

using nlohmann::json;

using bot::Vk_cmd_handler;
using bot::Cmds;
using std::to_string;
using std::vector;
using std::string;
using std::thread;


class Lp {
private:
  void    _loop         ();
  void    _get_lp_server();
  void    _errors_handle(long error_code);
  thread  _init_thread(const json& update);
  string  _server;
  string  _key;
  string  _ts;
  Logger  _logger{logfile, errfile};
  Cmds    _handler;

public:
  void init_bot();
  Lp(){};
};
