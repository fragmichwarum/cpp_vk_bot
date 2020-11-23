#include <thread>

#include "LongPoll.hpp"
#include "Curl.hpp"

#include "Stat.hpp"
#include "Cat.hpp"
#include "CRC32.hpp"
#include "Document.hpp"
#include "Face.hpp"
#include "Github.hpp"
#include "Picture.hpp"
#include "Video.hpp"
#include "Weather.hpp"
#include "Currency.hpp"

using std::string;
using std::thread;
using std::vector;

using bot::LongPoll;
using nlohmann::json;
using namespace bot::cURL;
using namespace bot::info;
using namespace bot::command;

void LongPoll::_init_invoker()
{
  _invoker->initCommand(new CatCommand);
  _invoker->initCommand(new StatCommand);
  _invoker->initCommand(new CRC32Command);
  _invoker->initCommand(new DocumentCommand);
  _invoker->initCommand(new PictureCommand);
  _invoker->initCommand(new VideoCommand);
  _invoker->initCommand(new WeatherCommand);
  _invoker->initCommand(new FaceCommand);
  _invoker->initCommand(new CurrencyCommand);
}

void LongPoll::_get_server() {
  json poll =
    json::parse(request(append_vkurl("groups.getLongPollServer"),
     {{ "group_id",     group_id     },
      { "random_id",    "0"          },
      { "access_token", access_token },
      { "v",            version      }}));

  if (not poll["error"]["error_code"].is_null()) {
    throw Vk_exception(poll["error"]["error_code"].get<long>());
  }

  _server = poll["response"]["server"];
  _key    = poll["response"]["key"];
  _ts     = poll["response"]["ts"];
}

void LongPoll::_singleThreadProcessing(const nlohmann::json& update) {
  _invoker->tryExecute(update);
}

void LongPoll::_multithreadProcessing(const nlohmann::json& updates) {
  vector<thread> threads;
  for (const json& update : updates) {
    threads.push_back(thread([=](){_invoker->tryExecute(update);}));
  }

  for (thread& th : threads) {
    th.join();
  }
}

void LongPoll::loop()
{
  _init_invoker();
  _get_server();

  while (true) {
    json lp =
      json::parse(request(_server + "?",
       {{ "act",  "a_check" },
        { "key",  _key      },
        { "ts",   _ts       },
        { "wait", "90"      }}));

    if (lp["updates"][0].is_null()) {
      _get_server();
      continue;
    }

    _ts = lp["ts"];

//    for (const json& update : lp["updates"]) {
//      _invoker->tryExecute(update);
//    }

    json update = lp["updates"];
    if (update.size() == 1) {
      _singleThreadProcessing(update[0]);
    } else {
      _multithreadProcessing(update);
    }
  }
}
