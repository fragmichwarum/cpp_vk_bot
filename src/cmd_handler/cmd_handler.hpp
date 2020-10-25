#pragma once

#include "../cmd_holder/cmd_holder.hpp"

using nlohmann::json;
using std::string;

class cmd_handler {
private:
  string  _message;
  long    _peer_id;
  long    _from_id;

public:
  explicit cmd_handler(const json& json);
  void init_cmds();
};
