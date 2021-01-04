#include "lib/include/api/long_poll_api.hpp"

#include "../bot/include/event_handler/message_event_handler.hpp"
#include "../bot/include/long_poll_api/long_poll_api.hpp"


bot::long_poll_handler::long_poll_handler()
  : lp_api                (std::make_unique<vk::long_poll_api>())
  , lp_data               (std::make_unique<vk::long_poll_data>())
  , message_event_handler (std::make_unique<bot::message_event_handler>())
{ }

bot::long_poll_handler::~long_poll_handler() = default;

void bot::long_poll_handler::loop()
{
  *lp_data = lp_api->get_server();

  while (true)
  {
    auto events = lp_api->listen(*lp_data);

    if (events.size() == 0) *lp_data = lp_api->get_server();

    for (auto&& event : events)
    {
      if (event.type() == "message_new") message_event_handler->process(event.get_message_event());

      lp_data->ts = event.ts();
    }
  }
}
