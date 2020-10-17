#include "../cmd_holder.hpp"
#include "../commands/lib/crc32.hpp"

void cmd_holder::crc32_command() {
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
    string crc32_str(stream.str());
    _message_send(crc32_str, USE_NICKNAME);
  }
}
