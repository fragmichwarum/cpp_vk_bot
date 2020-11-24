#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class CompleteCommand final : public ICommand
{
public:
  std::string execute([[maybe_unused]]const CommandParams& inputData) override;
  std::string description() const override;
  std::string trigger() const override;
 ~CompleteCommand()
  { }
};
} //namespace command
} //namespace bot
