#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class Host final : public ICommand
{
private:
  std::string getHostname(const std::string& ipAddress);

public:
  const std::string execute([[maybe_unused]]const CommandParams&) override;
  const std::string description() const override;
  const std::string trigger() const override;
  ~Host()
  { }
};
}
}
