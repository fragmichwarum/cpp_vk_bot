#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class Cat final : public ICommand
{
private:
  std::mutex _mutex;
  simdjson::dom::parser parser;
  Network* net = Network::getInstance();
  VkAPI* api = VkAPI::getInstance();

public:
  const std::string execute([[maybe_unused]]const CommandParams&) override;
  const std::string description() const override;
  const std::string trigger() const override;
 ~Cat()
  { }
};
} //namespace command
} //namespace bot
