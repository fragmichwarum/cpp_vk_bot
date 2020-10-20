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

public:
  cmd_handler(nlohmann::json& json);
  void init_cmds();
};

#endif // COMMAND_HANDLER_HPP
