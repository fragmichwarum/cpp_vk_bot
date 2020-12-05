#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class Ip final : public ICommand
{
private:
  std::string getIpAddress(const std::string& hostname);
  void ipLogging(const std::string& hostname, const std::string& ipAddress);

public:
  const std::string execute([[maybe_unused]]const CommandParams&) override;
  const std::string description() const override;
  const std::string trigger() const override;
  ~Ip()
  { }
};
}
}
