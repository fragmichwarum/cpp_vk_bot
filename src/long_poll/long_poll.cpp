#include "./long_poll.hpp"
#define LOG


void Lp::get_lp_server() {
  map<string, string> lp_parameters;
  lp_parameters["group_id"] = group_id;

  string response = Curl::generate_vk_query("groups.getLongPollServer", lp_parameters);
  auto lp = nlohmann::json::parse(Curl::request(response));

  if (not lp["error"]["error_code"].is_null()) {
    errors_handle(lp["error"]["error_code"].get<long>());
  }
  this->server  = lp["response"]["server"];
  this->key     = lp["response"]["key"];
  this->ts      = to_string((long)lp["response"]["ts"]);
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
    nlohmann::json lp = nlohmann::json::parse(Curl::request(Curl::generate_query(server + "?", lp_event)));
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
          handler.init_commands();
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
