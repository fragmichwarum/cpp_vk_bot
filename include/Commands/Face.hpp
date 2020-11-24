#pragma once

#include <mutex>

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class FaceCommand final : public ICommand
{
private:
  std::mutex _mutex;
public:
  std::string execute([[maybe_unused]]const CommandParams&) override;
  std::string description() const override;
  std::string trigger() const override;
 ~FaceCommand()
  { }
};
} //namespace command
} //namespace bot
