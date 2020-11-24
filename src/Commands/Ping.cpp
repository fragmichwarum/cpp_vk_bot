#include "Curl.hpp"
#include "Ping.hpp"

using std::string;
using std::to_string;
using bot::command::PingCommand;
using namespace bot::cURL;

string PingCommand::description() const
{
  return "проверить пинг до ВК 10 раз";
}

string PingCommand::trigger() const
{
  return "+пинг";
}

string PingCommand::execute([[maybe_unused]] const CommandParams& inputData)
{
  string ping = "Pinging users.get\n";
  float total = 0;
  for (uint8_t iter = 1; iter < 11; iter++) {
    clock_t now = clock();
    request(append_vkurl("users.get"), {
      { "user_ids",     "0"                },
      { "access_token", info::access_token },
      { "v",            info::version      }
    });
    float delay = (float)(clock() - now) / CLOCKS_PER_SEC * 10000;
    total += delay;
    ping += to_string(iter) + ". " + to_string(delay) + " ms\n";
  }
  ping += "-------------\n";
  ping += "Avg: " + to_string(total / 10.0f) + " ms.";
  return ping;
}
