#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class DocumentCommand final : public ICommand
{
public:
  std::string execute([[maybe_unused]]const CommandParams& inputData) override;
  std::string description() const override;
  std::string trigger() const override;
 ~DocumentCommand()
  { }
};
} //namespace command
} //namespace bot
