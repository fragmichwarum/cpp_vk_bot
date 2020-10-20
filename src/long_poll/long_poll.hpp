#ifndef LONG_POLL_HPP
#define LONG_POLL_HPP

#include "../cmd_handler/cmd_handler.hpp"

using std::string;
using nlohmann::json;

class Lp {
private:
  void   get_lp_server();
  void   loop();
  void   errors_handle(long error_code);
  string server;
  string key;
  string ts;
  Lp& operator=(Lp&) = delete;
public:
  void init_bot();
};

#endif // LONG_POLL_HPP
