#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class KickCommand final : public ICommand
{
private:
  long extractId(const std::string& username);
public:
  std::string execute([[maybe_unused]]const CommandParams& inputData) override;
  std::string description() const override;
  std::string trigger() const override;
 ~KickCommand()
  { }
};
} //namespace command
} //namespace bot
