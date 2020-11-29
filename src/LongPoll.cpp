#include <thread>

#include "LongPoll.hpp"
#include "Curl.hpp"
#include "Info.hpp"

#include "About.hpp"
#include "Cat.hpp"
#include "Complete.hpp"
#include "CRC32.hpp"
#include "Currency.hpp"
#include "Document.hpp"
#include "Face.hpp"
#include "Genius.hpp"
#include "Kick.hpp"
#include "Online.hpp"
#include "Picture.hpp"
#include "Ping.hpp"
#include "Role.hpp"
#include "Stat.hpp"
#include "Video.hpp"
#include "Weather.hpp"
#include "Who.hpp"

using bot::LongPoll;
using namespace nlohmann;
using namespace bot::command;

LongPoll::LongPoll()
  : _numThreads(std::thread::hardware_concurrency())
{ }

void LongPoll::_initInvoker()
{
  _invoker->initCommand(new AboutCommand);
  _invoker->initCommand(new CatCommand);
  _invoker->initCommand(new CompleteCommand);
  _invoker->initCommand(new DocumentCommand);
  _invoker->initCommand(new StatCommand);
  _invoker->initCommand(new CRC32Command);
  _invoker->initCommand(new FaceCommand);
  _invoker->initCommand(new GeniusCommand);
  _invoker->initCommand(new KickCommand);
  _invoker->initCommand(new OnlineCommand);
  _invoker->initCommand(new PictureCommand);
  _invoker->initCommand(new PingCommand);
  _invoker->initCommand(new RoleCommand);
  _invoker->initCommand(new VideoCommand);
  _invoker->initCommand(new WeatherCommand);
  _invoker->initCommand(new CurrencyCommand);
  _invoker->initCommand(new WhoCommand);
}

void LongPoll::_getServer()
{
  json poll =
    json::parse(cURL::request(cURL::appendVkUrl("groups.getLongPollServer"),
     {{ "group_id",     info::groupId      },
      { "random_id",    "0"                },
      { "access_token", info::accessToken  },
      { "v",            info::version      }}));

  if (not poll["error"]["error_code"].is_null()) {
    _errorLogger.print(poll["error"]["error_msg"].get<std::string>());
    _errorLogger.log(poll["error"]["error_msg"].get<std::string>());
    throw Vk_exception(poll["error"]["error_code"].get<long>());
  }

  _server = poll["response"]["server"];
  _key    = poll["response"]["key"];
  _ts     = poll["response"]["ts"];
}

void LongPoll::_singleThreadProcessing(const nlohmann::json& update)
{
  _invoker->tryExecute(std::move(update));
}

void LongPoll::_multithreadProcessing(const nlohmann::json& updates)
{
  std::vector<std::thread> threads;

  if (updates.size() <= _numThreads) {
    for (const json& update : updates) {
      threads.push_back(std::thread([&](){_invoker->tryExecute(std::move(update));}));
    }

    for (std::thread& th : threads) {
      th.join();
    }
    return;
  }

  threads.clear();
  for (std::size_t i = 0; i < updates.size(); i++) {
    threads.push_back(std::thread([&](){_invoker->tryExecute(updates[i]);}));
    if (i % _numThreads == 0) {
      for (std::thread& th : threads) {
        th.join();
      }
      threads.clear();
    }
  }
}

void LongPoll::loop()
{
  _initInvoker();
  _getServer();

  while (true) {
    json lp =
      json::parse(cURL::request(_server + "?",
       {{ "act",  "a_check" },
        { "key",  _key      },
        { "ts",   _ts       },
        { "wait", "90"      }}));

    if (lp["updates"][0].is_null()) {
      _getServer();
      continue;
    }

    _ts = lp["ts"];

    json updates = lp["updates"];
    if (updates.size() == 1) {
      _singleThreadProcessing(updates[0]);
    } else {
      _multithreadProcessing(updates);
    }
  }
}
