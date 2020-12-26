#include <array>
#include <cstring>
#include <memory>

#include "Stat.hpp"

constexpr std::uint8_t bot::command::Stat::access() const noexcept
{
  return static_cast<std::uint8_t>(bot::Access::user);
}

constexpr std::string_view bot::command::Stat::description() const noexcept
{
  return "Показать статистику";
}

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

std::string bot::command::Stat::execute([[maybe_unused]] const CommandParams& inputData, const Dependencies& /* unused */)
{
  return
    "Всего памяти: "      + procinfo("/proc/meminfo", "MemTotal:") + "KiB.\n"
    "Использовано ОЗУ: "  + procinfo("/proc/self/status", "VmRSS:") + "KiB.\n"
    "Потоков занято: "    + procinfo("/proc/self/status", "Threads:") + '\n' +
    "Аптайм: "            + os_exec("ps -eo lstart,etime,cmd | grep VK | awk '{print $6}' | head -1");
}
