#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class VideoCommand final : public ICommand
{
public:
  const std::string execute([[maybe_unused]]const CommandParams& inputData) override;
  const std::string description() const override;
  const std::string trigger() const override;
 ~VideoCommand()
  { }
};
} //namespace command
} //namespace bot
