#pragma once

#include <string>

namespace bot
{
struct CommandParams
{
  const std::string args;
  const long peer_id;
};

struct Dependencies
{
  class Repository* repository;
  class VkAPI* api;
  class Network* net;
};

class ICommand
{
public:
  virtual std::string execute(const CommandParams&, const Dependencies&) = 0;
  virtual std::string description() const = 0;
  virtual ~ICommand() = default;
};
} //namespace bot
