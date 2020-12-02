#include <thread>

#include "LongPoll.hpp"

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
  std::string response =
    cURL::request(cURL::appendVkUrl("groups.getLongPollServer"),
     {{ "group_id",     info::groupId      },
      { "random_id",    "0"                },
      { "access_token", info::accessToken  },
      { "v",            info::version      }});

  simdjson::padded_string padded_string = response;

  _server = std::string{parser.parse(padded_string)["response"]["server"].get_c_str()};
  _key = std::string{parser.parse(padded_string)["response"]["key"].get_c_str()};
  _ts = std::string{parser.parse(padded_string)["response"]["ts"].get_c_str()};
}

void LongPoll::_singleThreadProcessing(const simdjson::dom::object& update)
{
  _invoker->tryExecute(std::move(update));
}

void LongPoll::_multithreadProcessing(const simdjson::dom::array& updates)
{
  std::vector<std::thread> threads;

  if (updates.size() <= _numThreads) {
    for (const simdjson::dom::object& update : updates) {
      threads.push_back(std::thread([&](){_invoker->tryExecute(std::move(update));}));
    }
    for (std::thread& th : threads) {
      th.join();
    }
    return;
  }

  for (const simdjson::dom::object& update : updates) {
    threads.push_back(std::thread([&](){_invoker->tryExecute(update);}));
  }
  for (std::thread& thread : threads) {
    thread.join();
  }
}

void LongPoll::loop()
{
  _initInvoker();
  _getServer();

  while (true) {
    std::string response =
      cURL::request(_server + '?',
       {{ "act", "a_check" }, { "key",  _key },
        { "ts",  _ts       }, { "wait", "90" }});

    simdjson::dom::array updates = parser.parse(response)["updates"].get_array();

    if (updates.size() == 0) {
      _getServer();
      continue;
    }

    _ts = parser.parse(response)["ts"].get_c_str();

    if (updates.size() == 1) {
      _singleThreadProcessing(updates.at(0));
    } else {
      _multithreadProcessing(updates);
    }
  }
}
