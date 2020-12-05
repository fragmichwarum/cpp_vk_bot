#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class Stat final : public ICommand
{
private:
  static std::string lineparse(const std::string& line);
  static std::string procinfo(const std::string& filename, const std::string& param);
  static std::string os_exec(const std::string& cmd);

public:
  const std::string execute([[maybe_unused]]const CommandParams&) override;
  const std::string description() const override;
  const std::string trigger() const override;
  ~Stat()
  { }
};
}
}
