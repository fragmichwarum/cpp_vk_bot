#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class GeniusCommand final : public ICommand
{
public:
  const std::string execute([[maybe_unused]]const CommandParams& inputData) override;
  const std::string description() const override;
  const std::string trigger() const override;
 ~GeniusCommand()
  { }
};
} //namespace command
} //namespace bot
