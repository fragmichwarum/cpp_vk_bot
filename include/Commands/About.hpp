#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class About final : public ICommand
{
private:
  std::string buildTime;
  VkAPI* api = VkAPI::getInstance();

public:
  About();
  const std::string execute([[maybe_unused]]const CommandParams& inputData) override;
  const std::string description() const override;
  const std::string trigger() const override;
 ~About()
  { }
};
} //namespace command
} //namespace bot
