#include "./long_poll.hpp"
#define LOG

void Lp::get_lp_server() {
  map<string, string>  params;
  params["group_id"] = group_id;
  cmd_holder::append_vkparams(params);

  string method   = cmd_holder::append_vkurl("groups.getLongPollServer");
  string lp_query = Curl::curl_gen(method, params);
  json   poll     = json::parse(Curl::request(lp_query));

  if (not poll["error"]["error_code"].is_null()) {
    errors_handle(poll["error"]["error_code"].get<long>());
  }
  server = poll["response"]["server"];
  key    = poll["response"]["key"   ];
  ts     = to_string((long)poll["response"]["ts"]);
}

void Lp::loop() {
  get_lp_server();
  while (true) {
    map<string, string> lp_event = {
      { "act",  "a_check"  },
      { "key",   this->key },
      { "ts",    this->ts  },
      { "wait", "60"       }
    };
    json lp = json::parse(Curl::send_request(server +  "?", lp_event));
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
