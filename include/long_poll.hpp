#pragma once

#include "cmd_handler.hpp"
#include "error_handler.hpp"

namespace std
  {
    class thread;
  }

namespace bot
{
class Lp {
private:
  void         _loop         ();
  void         _get_lp_server();
  std::thread  _init_thread  (const nlohmann::json& update);
  std::string  _server;
  std::string  _key;
  std::string  _ts;
  Logger  _logger{logfile, errfile};
  bot::Cmds    _handler;

public:
  void init_bot();
};
} //namespace bot
