#pragma once

#include <unordered_map>

#include "../bot/include/commands/base_command.hpp"


namespace bot
{
class message_event_handler
{
public:
  explicit
  message_event_handler();
  ~message_event_handler();

  void process(const vk::event::message_new&);

private:
  std::unordered_map<std::string_view, std::unique_ptr<command::base_command>> command_collection;
};
}
