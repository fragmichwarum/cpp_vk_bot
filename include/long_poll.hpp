#pragma once

#include <thread>

#include "error_handler.hpp"
#include "cmd_handler.hpp"

namespace bot
{
class Long_poll_handler
{
private:
  std::string _server;
  std::string _key;
  std::string _ts;
  void _get_server();
  Cmd_handler _handler;

public:
  void loop();
};
} // namespace bot
