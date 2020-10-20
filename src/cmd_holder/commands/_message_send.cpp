#include "../cmd_holder.hpp"

void cmd_holder::_message_send(string text, bool use_nickname) {
  map<string, string> params;

  params["peer_id"] = to_string(_peer_id);

  if (_nickname != "" and use_nickname) {
    params["message"] = _nickname + ", " + text;
  } else {
    params["message"] = text;
  }
   append_vkparams(params);
  Curl::send_request(append_vkurl("messages.send"), params);
}
