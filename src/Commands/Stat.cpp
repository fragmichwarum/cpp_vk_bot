#include "Stat.hpp"

using std::string;
using std::to_string;
using nlohmann::json;
using bot::command::StatCommand;

string StatCommand::lineparse(const string& line)
{
  std::string result;
  for (char c : line) {
    if (isdigit(c)) {
      result += c;
    }
  }
  return result;
}

string StatCommand::procinfo(const std::string& filename, const std::string& param)
{
  FILE* file = fopen(filename.c_str(), "r");
  std::string result;
  std::array<char, 128> line;

  while (fgets(line.data(), 128, file) != NULL) {
    if (strncmp(line.data(), param.c_str(), param.size()) == 0) {
      return lineparse(line.data());
    }
  }
  fclose(file);
  return "";
}

string StatCommand::os_exec(const std::string& cmd)
{
  std::string result;
  std::array<char, 128> buffer;
  std::unique_ptr<FILE, decltype (&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

string StatCommand::description() const
{
  return "Показать статистику";
}

string StatCommand::trigger() const
{
  return "+стат";
}

string StatCommand::execute([[maybe_unused]] const CommandParams& inputData)
{
  return
    "Всего памяти: "      + procinfo("/proc/meminfo", "MemTotal:") + "KiB.\n"
    "Использовано ОЗУ: "  + procinfo("/proc/self/status", "VmRSS:") + "KiB.\n"
    "Потоков занято: "    + procinfo("/proc/self/status", "Threads:") + '\n' +
    "Аптайм: "            + os_exec("ps -eo lstart,etime,cmd | grep FactoryVK | awk '{print $6}' | head -1") +
    "Команд обработано: " + to_string(++_processedMessages);
}
