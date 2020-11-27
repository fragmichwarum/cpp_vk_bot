#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class AboutCommand final : public ICommand
{
private:
  std::string buildTime;
public:
  AboutCommand();
  const std::string execute([[maybe_unused]]const CommandParams& inputData) override;
  const std::string description() const override;
  const std::string trigger() const override;
 ~AboutCommand()
  { }
};
} //namespace command
} //namespace bot
