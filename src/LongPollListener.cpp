#include "LongPollListener.hpp"

void bot::LongPollListener::updateLongPollData()
{
  VkAPI::LongPollData data = api->getLongPollServer();

  server = data.server;
  key    = data.key;
  ts     = data.ts;
}

void bot::LongPollListener::processEvents(const simdjson::dom::array& updates)
{
  for (const simdjson::dom::object& update : updates) {
    eventHandler.tryProcessEvent(update);
  }
}

void bot::LongPollListener::loop()
{
  eventHandler.initAllCommands();
  updateLongPollData();

  while (true) {
    std::string response = api->listenLongPoll(key, server, ts);

    simdjson::dom::array updates = parser.parse(response)["updates"].get_array();

    if (updates.size() == 0) {
      updateLongPollData();
      continue;
    }

    ts = parser.parse(response)["ts"].get_c_str();
    processEvents(updates);
  }
}
