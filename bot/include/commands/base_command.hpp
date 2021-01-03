#pragma once

#include "lib/include/methods/messages.hpp"
#include "lib/include/events/common_event.hpp"


namespace bot::command
{
class base_command
{
public:
  virtual void execute(const vk::event::message_new& event) const = 0;
  virtual ~base_command() = default;

protected:
  vk::messages messages;
};
}
