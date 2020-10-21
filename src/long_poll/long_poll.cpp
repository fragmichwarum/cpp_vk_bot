#include "./long_poll.hpp"
#define LOG

void Lp::get_lp_server() {
  params body{{ "group_id", group_id }};
  append_vkparams(body);
  json poll = http_processing(append_vkurl("groups.getLongPollServer"), body);
  if (not poll["error"]["error_code"].is_null()) {
    errors_handle(poll["error"]["error_code"]);
  }
  server = poll["response"]["server"];
  key    = poll["response"]["key"];
  ts     = to_string(poll["response"]["ts"].get<long>());
}

void Lp::loop() {
  get_lp_server();
  while (true) {
    params lp_body = {
      { "act",  "a_check"  },
      { "key",   this->key },
      { "ts",    this->ts  },
      { "wait", "60"       }
    };
    json lp = http_processing(server + "?", lp_body);
  #ifdef LOG
    std::cout << lp << std::endl;
  #endif
    if (lp["updates"][0].is_null()) {
      get_lp_server();
    } else {
      ts = lp["ts"];
      for (auto update : lp["updates"]) {
        if (update["object"]["message"]["text"] != "") {
          cmd_handler handler(lp);
          handler.init_cmds();
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
