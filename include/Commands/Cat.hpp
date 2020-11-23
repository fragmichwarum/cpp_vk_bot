#pragma once

#include <mutex>

#include "../ICommand.hpp"
#include "../VkAPI.hpp"

namespace bot
{
namespace command
{
class CatCommand final : public ICommand
{
private:
  std::mutex _mutex;
public:
  std::string execute([[maybe_unused]]const CommandParams&) override;
  std::string description() const override;
  std::string trigger() const override;
 ~CatCommand()
  { }
};
} //namespace command
} //namespace bot
