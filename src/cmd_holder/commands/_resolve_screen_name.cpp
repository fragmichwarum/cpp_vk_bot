#include "../cmd_holder.hpp"

string cmd_holder::_resolve_screen_name(const string& screen_name) {
  map<string, string> answer_query = {
    make_pair("screen_name", screen_name)
  };
  string answer = Curl::request(Curl::generate_vk_query("utils.resolveScreenName", answer_query));
  auto parsed = nlohmann::json::parse(answer);
  long object_id = parsed["response"]["object_id"];
  return to_string(object_id);
}
