#pragma once

#include <string>

#include "VkAPI.hpp"
#include "Curl.hpp"

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
  virtual const std::string execute(const CommandParams&) = 0;
  virtual const std::string description() const = 0;
  virtual const std::string trigger() const = 0;
  virtual ~ICommand()
  { }
};
} //namespace bot
