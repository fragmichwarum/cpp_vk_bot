#include <time.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "Ip.hpp"

constexpr std::uint8_t bot::command::Ip::access() const noexcept
{
  return static_cast<std::uint8_t>(bot::Access::user);
}

constexpr std::string_view bot::command::Ip::description() const noexcept
{
	return "узнать ip адрес по хосту";
}

static std::string getIpAddress(const std::string& hostname)
{
  hostent* record = gethostbyname(hostname.c_str());
  if (record == NULL) return hostname + " unavailable.";

  in_addr* address = reinterpret_cast<in_addr*>(record->h_addr);
  std::string ipAddress = inet_ntoa(*address);

  return hostname + '(' + ipAddress + ')';
}

std::string bot::command::Ip::execute(const CommandParams& params, const Dependencies& /* unused */)
{
  if (params.args.empty()) return "Что-то пошло не так.";

  return getIpAddress(params.args);
}
