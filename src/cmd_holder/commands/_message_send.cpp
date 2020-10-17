#include "../cmd_holder.hpp"

void cmd_holder::_message_send(string text, bool use_nickname) {
  map<string, string> params;

  params["peer_id"] = to_string(_peer_id);

  if (_nickname != "" and use_nickname) {
    params["message"] = _nickname + ", " + text;
  } else {
    params["message"] = text;
  }
  Curl::request(Curl::generate_vk_query("messages.send", params));
}
