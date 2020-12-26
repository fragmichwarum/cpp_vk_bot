#include "Network.hpp"
#include "Ping.hpp"

constexpr std::uint8_t bot::command::Ping::access() const noexcept
{
  return static_cast<std::uint8_t>(bot::Access::user);
}

constexpr std::string_view bot::command::Ping::description() const noexcept
{
  return "проверить время ответа";
}

std::string bot::command::Ping::execute(const CommandParams&, const Dependencies& /* unused */)
{
  Network net;
  std::string pingdata = "Пинг до ВК:\n";
  float total = 0;

  for (std::uint8_t i = 1; i < 11; i++)
  {
    clock_t now = clock();
    net.request("https://api.vk.com/method/users.get?", {});

    float delay = static_cast<float>(clock() - now) / CLOCKS_PER_SEC * 10000;
    pingdata += std::to_string(i) + ". " + std::to_string(delay) + " ms.\n";

    total += delay;
  }
  return pingdata + "-------------\nAvg: " + std::to_string(total / 10.0f) + " ms.";
}
