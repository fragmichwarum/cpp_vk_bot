#include <thread>

#include "long_poll.hpp"

using bot::Long_poll_handler;
using nlohmann::json;
using namespace bot::cURL;
using namespace bot::info;

using std::thread;
using std::string;
using std::vector;

void Long_poll_handler::_get_server() {
  json poll =
    json::parse(request(append_vkurl("groups.getLongPollServer"),
     {{ "group_id",     group_id     },
      { "random_id",    "0"          },
      { "access_token", access_token },
      { "v",            version  }}));

  if (not poll["error"]["error_code"].is_null()) {
    throw Vk_error(poll["error"]["error_code"].get<long>());
  }

  _server = poll["response"]["server"];
  _key    = poll["response"]["key"];
  _ts     = poll["response"]["ts"];
}

void Long_poll_handler::loop() {
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
    } else {
      _ts = lp["ts"];

      if (lp["updates"].size() == 1) {
        _handler.init_cmds(lp["updates"][0]);
        continue;
      }

      vector<thread> threads;
      for (const json& update : lp["updates"]) {
        threads.push_back(thread([=]{_handler.init_cmds(update); }));
      }

      for (thread& th : threads) {
        th.join();
      }
    }
  }
}
