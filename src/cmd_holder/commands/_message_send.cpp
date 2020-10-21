#include "../cmd_holder.hpp"

void cmd_holder::_message_send(string text, bool use_nickname) {
  params body;

  body["peer_id"] = to_string(_peer_id);

  if (_nickname != "" and use_nickname) {
    body["message"] = _nickname + ", " + text;
  } else {
    body["message"] = text;
  }
   append_vkparams(body);
  Curl::send_request(append_vkurl("messages.send"), body);
}
