#include "../cmd_holder.hpp"

void cmd_holder::stat_cmd() {
  system("./process_uptime.sh vk > stat.txt");
  std::ifstream file;
  char* stat = nullptr;
  if (file) {
    file.open("stat.txt");
    file.seekg(0, std::ios::end);
    int length = file.tellg();
    file.seekg(0, std::ios::beg);
    stat = new char[length];
    file.read(stat, length);
    file.close();
  }
  string bot_info = "Аптайм: ";
  bot_info += stat;
  bot_info += "\n";

  system("./mem.sh vk > mem.txt");
  char* memusage = nullptr;
  if (file) {
    file.open("mem.txt");
    file.seekg(0, std::ios::end);
    int length = file.tellg();
    file.seekg(0, std::ios::beg);
    memusage = new char[length];
    file.read(memusage, length);
    file.close();
  }
  bot_info += "Общая статистика: \n";
  bot_info += memusage;
  _message_send(bot_info, NOT_USE_NICKNAME);
}
