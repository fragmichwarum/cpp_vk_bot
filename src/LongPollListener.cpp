#include <simdjson.h>

#include "VkAPI.hpp"
#include "EventHandler.hpp"
#include "LongPollListener.hpp"


bot::LongPollListener::LongPollListener()
  : eventHandler(std::make_unique<EventHandler>())
  , api(std::make_unique<VkAPI>())
{ }

bot::LongPollListener::~LongPollListener() = default;

void bot::LongPollListener::updateLongPollData_()
{
  LongPollData data = api->getLongPollServer();

  server = data.server;
  key    = data.key;
  ts     = data.ts;
}

void bot::LongPollListener::processEvents_(const simdjson::dom::array& updates)
{
  for (const simdjson::dom::object& update : updates) {
    eventHandler->tryProcessEvent(update);
  }
}

void bot::LongPollListener::loop()
{
  updateLongPollData_();
  simdjson::dom::parser parser;

  while(true) {
    std::string lp = api->listenLongPoll(key, server, ts);

    simdjson::dom::array updates = parser.parse(lp)["updates"].get_array();

    if (updates.size() == 0) { updateLongPollData_(); continue; }

    ts = parser.parse(lp)["ts"].get_c_str();
    processEvents_(updates);
  }
}
