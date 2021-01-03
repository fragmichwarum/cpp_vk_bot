#pragma once

#include <list>
#include "lib/include/api/base_object.hpp"
#include "lib/include/events/common_event.hpp"

namespace vk
{
struct long_poll_data
{
  std::string key;
  std::string server;
  std::string ts;
};

class long_poll_api : private base_object
{
public:
  long_poll_api();
  long_poll_data get_server() const;
  std::list<event::common> listen(const long_poll_data& data) const;

private:
  std::string _group_id;
};
}
