#pragma once

#include <memory>


namespace vk    { class long_poll_api;  }
namespace vk    { struct long_poll_data; }
namespace bot   { class message_event_handler; }

namespace bot
{
class long_poll_handler
{
public:
  explicit
  long_poll_handler();
  ~long_poll_handler();

  void loop();

private:
  std::unique_ptr<vk::long_poll_api> lp_api;
  std::unique_ptr<vk::long_poll_data> lp_data;
  std::unique_ptr<bot::message_event_handler> message_event_handler;
};
}
