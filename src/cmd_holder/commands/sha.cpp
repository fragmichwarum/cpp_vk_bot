#include "../cmd_holder.hpp"
#include "../commands/lib/sha256.hpp"

void cmd_holder::sha256_command() {
  if (_message == "+sha256") {
    _empty_query();
  } else {
    SHA256 sha(
      _message.substr(
        /* select from index:*/ _splitted_message[0].size() + 1,
        /* with length:      */ _message.size() - 1)
        /* and convert       */ .c_str()
    );
    _message_send(sha.hash(), USE_NICKNAME);
  }
}
