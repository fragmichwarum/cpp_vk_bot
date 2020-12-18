#include "VkAPI.hpp"
#include "EventHandler.hpp"
#include "LongPollListener.hpp"

bot::VkAPI* bot::LongPollListener::api_ = new bot::VkAPI;
bot::EventHandler* bot::LongPollListener::eventHandler_ = new bot::EventHandler;

void bot::LongPollListener::updateLongPollData_()
{
  LongPollData data = api_->getLongPollServer();

  server_ = data.server;
  key_    = data.key;
  ts_     = data.ts;
}

void bot::LongPollListener::processEvents_(const simdjson::dom::array& updates)
{
  for (const simdjson::dom::object& update : updates) {
    eventHandler_->tryProcessEvent(update);
  }
}

void bot::LongPollListener::loop()
{
  updateLongPollData_();
  simdjson::dom::parser parser_;

  while (true) {
    std::string response = api_->listenLongPoll(key_, server_, ts_);

    simdjson::dom::array updates = parser_.parse(response)["updates"].get_array();

    if (updates.size() == 0) {
      updateLongPollData_();
      continue;
    }

    ts_ = parser_.parse(response)["ts"].get_c_str();
    processEvents_(updates);
  }
}

bot::LongPollListener::~LongPollListener()
{
  delete api_;
}
