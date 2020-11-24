#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class VideoCommand final : public ICommand
{
public:
  std::string execute([[maybe_unused]]const CommandParams& inputData) override;
  std::string description() const override;
  std::string trigger() const override;
 ~VideoCommand()
  { }
};
} //namespace command
} //namespace bot
