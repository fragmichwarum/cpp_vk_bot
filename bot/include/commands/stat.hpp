#pragma once

#include "../bot/include/commands/base_command.hpp"


namespace bot::command
{
class stat final : public base_command
{
public:
  void execute(const vk::event::message_new& event) const override;
};
}
