#include <netdb.h>
#include <arpa/inet.h>
#include <ares.h>

#include "../bot/include/3rd_party/string_utils.hpp"
#include "../bot/include/commands/host.hpp"


std::string hostname;

static void dns_callback(void* /* unused */, int status, int /* unused */, hostent* host) noexcept
{
  if(status == ARES_SUCCESS) hostname = host->h_name;
  else hostname = "lookup failed: " + std::to_string(status);
}

static void mainLoop(ares_channel& channel)
{
  int nfds = 0;
  timeval tv;
  timeval* tvp;
  fd_set readers;
  fd_set writers;

  while(true) {
    FD_ZERO(&readers);
    FD_ZERO(&writers);

    nfds = ares_fds(channel, &readers, &writers);
    if (nfds == 0) break;

    tvp = ares_timeout(channel, NULL, &tv);
    select(nfds, &readers, &writers, NULL, tvp);
    ares_process(channel, &readers, &writers);
  }
}

static std::string get_hostname(std::string_view ipAddress)
{
  struct in_addr ip;
  ares_channel channel;

  inet_aton(ipAddress.data(), &ip);
  if (ares_init(&channel) != ARES_SUCCESS) return "Failed.";
  ares_gethostbyaddr(channel, &ip, sizeof ip, AF_INET, dns_callback, NULL);
  mainLoop(channel);
  return hostname;
}

void bot::command::host::execute(const vk::event::message_new& event) const
{
  std::string args = third_party::string_utils::cut_first(event.text());

  if (args.empty()) { messages.send(event.peer_id(), "Задана пустая строка."); return; }

  messages.send(event.peer_id(), get_hostname(args));
}
