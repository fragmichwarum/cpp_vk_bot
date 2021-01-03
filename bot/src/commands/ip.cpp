#include <time.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "bot/include/3rd_party/string_utils.hpp"
#include "bot/include/commands/ip.hpp"


static std::string get_ip(const std::string& hostname)
{
  hostent* record = gethostbyname(hostname.c_str());
  if (record == NULL) return hostname + " unavailable.";

  in_addr* address = reinterpret_cast<in_addr*>(record->h_addr);
  std::string ipAddress = inet_ntoa(*address);

  return hostname + '(' + ipAddress + ')';
}

void bot::command::ip::execute(const vk::event::message_new& event) const
{
  std::string args = third_party::string_utils::cut_first(event.text());

  if (args.empty()) { messages.send(event.peer_id(), "Задана пустая строка."); return; }

  messages.send(event.peer_id(), get_ip(args));
}
