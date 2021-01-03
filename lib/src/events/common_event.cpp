#include "lib/include/events/common_event.hpp"

vk::event::common::common(std::string_view ts, std::string_view update_type, std::string_view raw_json)
  : _ts(ts)
  , _update_type(update_type)
  , _raw_json(raw_json)
{ }

vk::event::message_new vk::event::common::get_message_event() const
{
  return vk::event::message_new(_raw_json);
}

vk::event::wall_post_new vk::event::common::get_wall_post_event() const
{
  return vk::event::wall_post_new(_raw_json);
}

std::string vk::event::common::type() const noexcept
{
  return _update_type;
}

std::string vk::event::common::ts() const noexcept
{
  return _ts;
}

std::string vk::event::common::dump() const noexcept
{
  return _raw_json;
}
