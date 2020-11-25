#include "Curl.hpp"
#include "Ping.hpp"
#include "Info.hpp"

using bot::command::PingCommand;

std::string PingCommand::description() const
{
  return "проверить пинг до ВК 10 раз";
}

std::string PingCommand::trigger() const
{
  return "+пинг";
}

std::string PingCommand::execute([[maybe_unused]] const CommandParams& inputData)
{
  std::string ping = "Pinging users.get\n";
  float total = 0;
  for (uint8_t iter = 1; iter < 11; iter++) {
    clock_t now = clock();
    cURL::request(cURL::appendVkUrl("users.get"), {
      { "user_ids",     "0"                },
      { "access_token", info::accessToken  },
      { "v",            info::version      }
    });
    float delay = (float)(clock() - now) / CLOCKS_PER_SEC * 10000;
    total += delay;
    ping += std::to_string(iter) + ". " + std::to_string(delay) + " ms\n";
  }
  ping += "-------------\n";
  ping += "Avg: " + std::to_string(total / 10.0f) + " ms.";
  return ping;
}
