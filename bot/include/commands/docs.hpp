#pragma once

#include <memory>

#include "../bot/include/commands/base_command.hpp"


namespace vk { class docs; }

namespace bot::command
{
class docs final : public base_command
{
public:
  explicit docs();
  ~docs();
  void execute(const vk::event::message_new& event) const override;

private:
  std::unique_ptr<vk::docs> doc;
};
}
