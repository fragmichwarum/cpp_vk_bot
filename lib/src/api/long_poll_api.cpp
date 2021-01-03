#include <simdjson.h>

#include "lib/include/lib/network.hpp"
#include "lib/include/api/long_poll_api.hpp"

vk::long_poll_api::long_poll_api()
{
  std::string response =
  network->request(append_url("groups.getById"), group_params({}));

  _group_id = std::to_string(static_cast<long>(parser->parse(response)["response"].at(0)["id"]));
}

vk::long_poll_data vk::long_poll_api::get_server() const
{
  std::string response =
  call("groups.getLongPollServer", group_params({
    { "group_id",   _group_id },
    { "random_id",  "0" }
  }));

  simdjson::dom::object server = parser->parse(response)["response"];

  return {
    static_cast<std::string>(server["key"]),
    static_cast<std::string>(server["server"]),
    static_cast<std::string>(server["ts"])
  };
}

std::list<vk::event::common> vk::long_poll_api::listen(const long_poll_data& data) const
{
  std::string raw_json = network->request(data.server + '?', {{"act", "a_check"}, {"key", data.key}, {"ts", data.ts}, {"wait", "1"}});

  simdjson::dom::array raw_updates = parser->parse(raw_json)["updates"].get_array();

  std::list<vk::event::common> updates;

  for (const simdjson::dom::object& update : raw_updates)
  {
    updates.push_back({
      static_cast<std::string>(parser->parse(raw_json)["ts"]),
      static_cast<std::string>(update["type"]),
      simdjson::to_string(update)
    });
  }
  return updates;
}
