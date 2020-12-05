#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class Who final : public ICommand
{
private:
  VkAPI* api = VkAPI::getInstance();

public:
  const std::string execute([[maybe_unused]]const CommandParams& inputData) override;
  const std::string description() const override;
  const std::string trigger() const override;
 ~Who()
  { }
};
} //namespace command
} //namespace bot
