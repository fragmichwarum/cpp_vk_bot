#include "VkAPI.hpp"
#include "EventHandler.hpp"
#include "LongPollListener.hpp"

bot::VkAPI* api = new bot::VkAPI;
bot::EventHandler* bot::LongPollListener::eventHandler = new bot::EventHandler;

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

  while(true) {
    std::string lp = api->listenLongPoll(key, server, ts);

    simdjson::dom::parser parser;
    simdjson::dom::array updates = parser.parse(lp)["updates"].get_array();
    ts = parser.parse(lp)["ts"].get_c_str();

    if (updates.size() == 0) { updateLongPollData_(); continue; }

    processEvents_(updates);
  }
}

bot::LongPollListener::~LongPollListener()
{
  delete api;
}
