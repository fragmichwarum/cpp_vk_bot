#include "./long_poll.hpp"

using namespace cURL;
using std::to_string;
using std::thread;
using nlohmann::json;

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

      std::vector<std::thread> threads;
      uint32_t updates_count = lp["updates"].size();

      if (updates_count == 1) {
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

      for (uint8_t i = 0; i < updates_count; i++) {
        json update = lp["updates"][i]["object"]["message"];
        if (not update.is_null() and update["text"] != "") {
          threads.push_back(handler.init_thread(
            update["text"],
            update["peer_id"],
            update["from_id"]
          ));
        }
      }

      for (uint8_t i = 0; i < updates_count; i++) {
        if (lp["updates"][i]["object"]["message"]["text"] != "") {
          threads[i].join();
        }
      }
    }
  }
}

void Lp::init_bot() {
  Database db;
  db.open();
  db.init_table();
  loop();
}
