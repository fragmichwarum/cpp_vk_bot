#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class WhoCommand final : public ICommand
{
public:
  const std::string execute([[maybe_unused]]const CommandParams& inputData) override;
  const std::string description() const override;
  const std::string trigger() const override;
 ~WhoCommand()
  { }
};
} //namespace command
} //namespace bot
