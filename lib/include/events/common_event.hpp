#pragma once

#include "lib/include/events/message_new.hpp"
#include "lib/include/events/wall_post_new.hpp"


namespace vk::event
{
class common
{
public:
  common(std::string_view ts, std::string_view update_type, std::string_view raw_json);

  std::string type() const noexcept;
  std::string ts() const noexcept;
  std::string dump() const noexcept;

  message_new get_message_event() const;
  wall_post_new get_wall_post_event() const;

private:
  std::string _ts;
  std::string _update_type;
  std::string _raw_json;
};
}
