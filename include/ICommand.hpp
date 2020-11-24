#pragma once

#include <string>
#include "VkAPI.hpp"

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
  virtual std::string execute([[maybe_unused]] const CommandParams&) = 0;
  virtual std::string description() const = 0;
  virtual std::string trigger() const = 0;
  virtual ~ICommand()
  { }
};
} //namespace bot
