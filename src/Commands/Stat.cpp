#include "Stat.hpp"

std::string bot::command::Stat::lineparse(const std::string& line)
{
  std::string result;
  for (char c : line) {
    if (isdigit(c)) {
      result += c;
    }
  }
  return result;
}

std::string bot::command::Stat::procinfo(const std::string& filename, const std::string& param)
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

std::string bot::command::Stat::os_exec(const std::string& cmd)
{
  std::string result;
  std::array<char, 128> buffer;
  std::unique_ptr<FILE, decltype (&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

const std::string bot::command::Stat::description() const
{
  return "Показать статистику";
}

const std::string bot::command::Stat::trigger() const
{
  return "+стат";
}

const std::string bot::command::Stat::execute([[maybe_unused]] const CommandParams& inputData)
{
  return
    "Всего памяти: "      + procinfo("/proc/meminfo", "MemTotal:") + "KiB.\n"
    "Использовано ОЗУ: "  + procinfo("/proc/self/status", "VmRSS:") + "KiB.\n"
    "Потоков занято: "    + procinfo("/proc/self/status", "Threads:") + '\n' +
    "Аптайм: "            + os_exec("ps -eo lstart,etime,cmd | grep FactoryVK | awk '{print $6}' | head -1");
}
