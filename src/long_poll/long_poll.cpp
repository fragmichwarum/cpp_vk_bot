#include "./long_poll.hpp"

#define BOT
//#define STRESS_TEST

void Lp::_get_lp_server() {
  params body;
  body["group_id"] = group_id;
  append_vkparams(body);

  json poll = json::parse(request(append_vkurl("groups.getLongPollServer"), body));

  if (not poll["error"]["error_code"].is_null()) {
    long errcode = poll["error"]["error_code"];
    _errors_handle(errcode);
  }

  _server = poll["response"]["server"];
  _key    = poll["response"]["key"];
  _ts     = poll["response"]["ts"];
}

#ifdef STRESS_TEST
void Lp::_loop() {
  while (true) {
    _handler.stress_test("2000000001");
  }
}
#endif //STRESS_TEST

#ifdef BOT
thread Lp::_init_thread(const json &update) {
  return thread([=]{ _handler.init_cmds(update); });
}

void Lp::_loop() {
  _get_lp_server();
  while (true) {
    params lp_body;
    lp_body["act"]  = "a_check";
    lp_body["key"]  = _key;
    lp_body["ts"]   = _ts;
    lp_body["wait"] = "60";

    json lp = json::parse(request(_server + "?", lp_body));
    if (lp["updates"][0].is_null()) {
      _get_lp_server();
    } else {
      _ts = lp["ts"];

      if (lp["updates"].size() == 1) {
        _handler.init_cmds(lp["updates"][0]);
        continue;
      }

      vector<std::thread> threads;
      for (const json& update : lp["updates"]) {
        threads.push_back(_init_thread(update));
      }

      for (std::thread& th : threads) {
        th.join();
      }
    }
  }
}
#endif //BOT

void Lp::init_bot() {
  Database db;
  db.open();
  db.init_table();
  _loop();
}
