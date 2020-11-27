#pragma once

#include <string>

namespace bot
{
struct CommandParams
{
  const std::string args;
  const long peer_id;
};

class ICommand
{
public:
  virtual const std::string execute([[maybe_unused]] const CommandParams&) = 0;
  virtual const std::string description() const = 0;
  virtual const std::string trigger() const = 0;
  virtual ~ICommand()
  { }
};
} //namespace bot
