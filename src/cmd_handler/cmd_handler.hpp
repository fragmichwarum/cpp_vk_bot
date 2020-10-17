#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP

#include "../cmd_holder/cmd_holder.hpp"
#include "../logger/logger.hpp"

using std::string;
using std::to_string;

class cmd_handler {
private:
  string  _message;
  long    _peer_id;
  long    _from_id;
  cmd_handler() = delete;
  cmd_handler& operator=(cmd_handler&) = delete;

public:
  cmd_handler(nlohmann::json& json);
  void init_commands();
};

#endif // COMMAND_HANDLER_HPP
