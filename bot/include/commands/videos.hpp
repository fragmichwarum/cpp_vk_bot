#pragma once

#include <memory>

#include "../bot/include/commands/base_command.hpp"


namespace vk { class messages;     }
namespace vk { class video;        }

namespace bot::command
{
class videos final : public base_command
{
public:
  explicit videos();
  ~videos();
  void execute(const vk::event::message_new& event) const override;

private:
  std::unique_ptr<vk::messages> messages;
  std::unique_ptr<vk::video>    video;
};
}
