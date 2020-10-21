#include "../cmd_holder.hpp"

namespace {
  string lineparse(const string& line) {
    string result;
    for (char c : line) {
      if (isdigit(c)) {
        result += c;
      }
    }
    return result;
  }

  string memusage() {
    FILE* file = fopen("/proc/self/status", "r");
    string result;
    char line[128];

    while (fgets(line, 128, file) != NULL) {
      if (strncmp(line, "VmRSS:", 6) == 0) {
        return lineparse(line);
      }
    }
    fclose(file);
    return "";
  }
}

void cmd_holder::stat_cmd() {
  _message_send("Использовано ОЗУ: " + memusage() + " KiB.", NOT_USE_NICKNAME);
  /* cpuinfo... */
  /* uptime.... */
}
