#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class PingCommand final : public ICommand
{
public:
  const std::string execute([[maybe_unused]]const CommandParams& inputData) override;
  const std::string description() const override;
  const std::string trigger() const override;
 ~PingCommand()
  { }
};
} //namespace command
} //namespace bot
