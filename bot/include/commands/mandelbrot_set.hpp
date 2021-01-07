#pragma once

#include "../bot/include/commands/base_command.hpp"


namespace vk { class photos; }

namespace bot::command
{
class mandelbrot_set final : public base_command
{
public:
  explicit mandelbrot_set();
  ~mandelbrot_set();
  void execute(const vk::event::message_new& event) const override;

private:
  std::unique_ptr<vk::photos> photos;
};
}
