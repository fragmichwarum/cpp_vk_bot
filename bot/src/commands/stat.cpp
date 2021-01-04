#include <cstring>

#include "../bot/include/commands/stat.hpp"


static std::string lineparse(std::string_view line)
{
  std::string result;
  for (char c : line) {
    if (isdigit(c)) {
      result += c;
    }
  }
  return result;
}

static std::string procinfo(const std::string& filename, const std::string& param)
{
  FILE* file = fopen(filename.c_str(), "r");
  std::string result;
  std::array<char, 128> line;

  while (fgets(line.data(), 128, file) != NULL) {
    if (strncmp(line.data(), param.c_str(), param.size()) == 0) {
      fclose(file);
      return lineparse(line.data());
    }
  }
  fclose(file);
  return "";
}

static std::string os_exec(std::string_view cmd)
{
  std::string result;
  std::array<char, 128> buffer;
  std::unique_ptr<FILE, decltype (&pclose)> pipe(popen(cmd.data(), "r"), pclose);
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

void bot::command::stat::execute(const vk::event::message_new& event) const
{
  messages.send(
    event.peer_id(),
    "Всего памяти: "      + procinfo("/proc/meminfo", "MemTotal:") + "KiB.\n"
    "Использовано ОЗУ: "  + procinfo("/proc/self/status", "VmRSS:") + "KiB.\n"
    "Потоков занято: "    + procinfo("/proc/self/status", "Threads:") + '\n' +
    "Аптайм: "            + os_exec("ps -eo lstart,etime,cmd | grep bot | awk '{print $6}' | head -1")
  );
}
