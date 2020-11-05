#include "./long_poll.hpp"

#define BOT
//#define STRESS_TEST

void Lp::get_lp_server() {
  params body;
  body["group_id"] = group_id;
  append_vkparams(body);

  json poll = json::parse(request(append_vkurl("groups.getLongPollServer"), body));

  if (not poll["error"]["error_code"].is_null()) {
    long errcode = poll["error"]["error_code"];
    errors_handle(errcode);
  }

  server = poll["response"]["server"];
  key    = poll["response"]["key"];
  ts     = to_string(poll["response"]["ts"].get<long>());
}

thread Cmd_handler::init_thread(
  const string& message,
  const long&   peer_id,
  const long&   from_id)
{
  return thread([=]{ init_cmds(message, peer_id, from_id); });
}

#ifdef STRESS_TEST
void Lp::loop() {
  while (true) {
    params lp_body;
    std::this_thread::sleep_for (std::chrono::milliseconds(750));
    handler.stress_test("2000000001");
  }
}
#endif //STRESS_TEST

#ifdef BOT
void Lp::loop() {
  get_lp_server();
  while (true) {
    params lp_body;
    lp_body["act"]  = "a_check";
    lp_body["key"]  = key;
    lp_body["ts"]   = ts;
    lp_body["wait"] = "60";

    json lp = json::parse(request(server + "?", lp_body));
    if (lp["updates"][0].is_null()) {
      get_lp_server();
    } else {
      ts = lp["ts"];

      vector<std::thread> threads;
      uint32_t updates_count = lp["updates"].size();

      if (updates_count == 1) {
        ts = lp["ts"];
        json update = lp["updates"][0]["object"]["message"];
        if (update["text"] != "") {
          handler.init_cmds(
            update["text"],
            update["peer_id"],
            update["from_id"]
          );
          continue;
        }
      }

      for (uint32_t i = 0; i < updates_count; i++) {
        json update = lp["updates"][i]["object"]["message"];
        if (not update.is_null() and update["text"] != "") {
          threads.push_back(handler.init_thread(
            update["text"],
            update["peer_id"],
            update["from_id"]
          ));
        }
      }

      for (uint32_t i = 0; i < updates_count; i++) {
        long __ts = std::stol(lp["ts"].get<string>());
        __ts -= updates_count - i - 1;
        ts = to_string(__ts);
//        ts = lp["ts"];
        /* ПОШЁЛ НАХУЙ ЭТОТ ЛОНГ ПОЛЛ Я ЕГО РОТ НАОБОРОТ */
        if (threads.size() > 0 and lp["updates"][i]["object"]["message"]["text"] != "") {
          threads[i].join();
        }
      }
    }
  }
}
#endif //BOT

void Lp::init_bot() {
  Database db;
  db.open();
  db.init_table();
  loop();
}
