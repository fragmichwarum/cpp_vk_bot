#include "../cmd_holder.hpp"
#include "../commands/lib/crc32.hpp"

void cmd_holder::crc32_cmd() {
  if (_message == "+crc32") {
    _empty_query();
  } else {
    unsigned long crc32 =
      CRC32::crc32gen(
        _message.substr(
          /* select from index:*/ _splitted_message[0].size() + 1,
          /* with length:      */ _message.size() - 1)
          /* and convert       */ .c_str()
      );
    std::stringstream stream;
    stream << "0x" << std::hex << crc32;
    _message_send(stream.str(), USE_NICKNAME);
  }
}
