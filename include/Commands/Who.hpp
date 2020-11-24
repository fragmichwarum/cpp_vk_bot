#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class WhoCommand final : public ICommand
{
public:
  std::string execute([[maybe_unused]]const CommandParams& inputData) override;
  std::string description() const override;
  std::string trigger() const override;
 ~WhoCommand()
  { }
};
} //namespace command
} //namespace bot
