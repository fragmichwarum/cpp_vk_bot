#include "../cmd_holder.hpp"

namespace {
  using std::array;
  using std::unique_ptr;
  using std::runtime_error;

  string os_exec(const string& cmd) {
    array<char, 128> buffer;
    string result;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (not pipe) {
      throw runtime_error("popen() failure");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
      result += buffer.data();
    }
    return result;
  }
}

void cmd_holder::os_cmd() {
  string cmd =
    _message.substr(
      /* select from index:*/ _splitted_message[0].size() + 1,
      /* with length:      */ _message.size() - 1)
      /* and convert       */ .c_str();
  _message_send(os_exec(cmd), NOT_USE_NICKNAME);
}
