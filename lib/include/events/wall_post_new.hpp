#pragma once

#include <string_view>

namespace vk::event
{
class wall_post_new
{
public:
  wall_post_new(std::string_view raw_json);

  std::int64_t id()      const noexcept;
  std::int64_t from_id() const noexcept;

private:
  std::int64_t _id;
  std::int64_t _from_id;
};
}
