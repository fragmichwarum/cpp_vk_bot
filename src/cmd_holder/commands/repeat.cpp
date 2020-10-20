#include "../cmd_holder.hpp"

void cmd_holder::repeat_cmd() {
  if (_message != "+!") {
  string to_repeat =
    _message.substr(
      /* select from index:*/ _splitted_message[0].size() + 1,
      /* with length:      */ _message.size() - 2)
      /* and convert       */ .c_str();
  _message_send(to_repeat, NOT_USE_NICKNAME);
  } else {
    _empty_query();
  }
}
