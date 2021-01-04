#pragma once

#include <memory>

#include "../bot/include/commands/base_command.hpp"


namespace vk { class photos; }

namespace bot::command
{
class pics final : public base_command
{
public:
  explicit pics();
  ~pics();
  void execute(const vk::event::message_new& event) const override;

private:
  std::unique_ptr<vk::photos> photos;
};
}
