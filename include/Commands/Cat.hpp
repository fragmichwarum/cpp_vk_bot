#pragma once

#include <mutex>

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class CatCommand final : public ICommand
{
private:
  std::mutex _mutex;
public:
  const std::string execute([[maybe_unused]]const CommandParams&) override;
  const std::string description() const override;
  const std::string trigger() const override;
 ~CatCommand()
  { }
};
} //namespace command
} //namespace bot
