#include <time.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fstream>

#include "Ip.hpp"

const std::string bot::command::Ip::description() const
{
	return "узнать ip адрес по хосту";
}

const std::string bot::command::Ip::trigger() const
{
	return "+ip";
}

void bot::command::Ip::ipLogging(const std::string& hostname, const std::string& ipAddress)
{
	time_t rawtime;
	tm* ptm = gmtime(&rawtime);

	std::ofstream ipaddr_log("ipaddr.log", std::ios::app);
	ipaddr_log << (ptm->tm_hour % 24) << ":" << (ptm->tm_min) << " " << hostname << " (" << ipAddress << ")\n";
	ipaddr_log.close();
}

std::string bot::command::Ip::getIpAddress(const std::string& hostname)
{
  hostent* record = gethostbyname(hostname.c_str());
  if (record == NULL) return hostname + " unavailable.";

  in_addr* address = reinterpret_cast<in_addr*>(record->h_addr);
  std::string ipAddress = inet_ntoa(*address);

  ipLogging(hostname, ipAddress);
  return hostname + '(' + ipAddress + ')';
}

const std::string bot::command::Ip::execute(const CommandParams& params)
{
  if (params.args.empty()) return "Что-то пошло не так.";

  std::cout << std::endl << getIpAddress(params.args) << std::endl;
  return getIpAddress(params.args);
}
