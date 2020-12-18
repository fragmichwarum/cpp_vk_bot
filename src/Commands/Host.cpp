#include <netdb.h>
#include <arpa/inet.h>
#include <ares.h>

#include "Host.hpp"

std::string hostName_;

std::string bot::command::Host::description() const
{
  return "узнать хост по ip адресу";
}

static void dnsCallback([[maybe_unused]] void* arg, [[maybe_unused]] int status, [[maybe_unused]] int timeouts, hostent* host)
{
  if(status == ARES_SUCCESS) hostName_ = host->h_name;
  else hostName_ = "lookup failed: " + std::to_string(status);
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

static std::string getHostname(const std::string& ipAddress)
{
  struct in_addr ip;
  ares_channel channel;

  inet_aton(ipAddress.c_str(), &ip);
  if (ares_init(&channel) != ARES_SUCCESS) return "Failed.";
  ares_gethostbyaddr(channel, &ip, sizeof ip, AF_INET, dnsCallback, NULL);
  mainLoop(channel);
  return hostName_;
}

std::string bot::command::Host::execute(const CommandParams& params, [[maybe_unused]] const Dependencies& deps)
{
  if (params.args.empty()) return "Что-то пошло не так.";

  return getHostname(params.args);
}
