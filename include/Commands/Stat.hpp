#pragma once

#include "../ICommand.hpp"
#include "../VkAPI.hpp"

namespace bot
{
namespace command
{
class StatCommand final : public ICommand
{
private:
  mutable uint8_t _processedMessages = 0;
  static std::string lineparse(const std::string& line);
  static std::string procinfo(const std::string& filename, const std::string& param);
  static std::string os_exec(const std::string& cmd);
public:
  std::string execute([[maybe_unused]]const CommandParams&) override;
  std::string description() const override;
  std::string trigger() const override;
 ~StatCommand()
  { }
};
} //namespace command
} //namespace bot
