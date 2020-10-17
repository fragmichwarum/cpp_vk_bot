#include "cmd_handler.hpp"

namespace {
  bool starts_with(const std::string& str, char ch) {
    return str[0] == ch;
  }
}

cmd_handler::cmd_handler(nlohmann::json& json)
{
  auto response = json["updates"][0]["object"]["message"];
  _message = response["text"];
  _peer_id = response["peer_id"];
  _from_id = response["from_id"];

  Logger logger{"/home/machen/log.txt"};
  if (starts_with(_message, '+')) {
    logger.write_log(_message);
  }
}

void cmd_handler::init_commands()
{
  map<string, void(cmd_holder::*)()> cmds;

  cmds = {
    { "+помощь",   &cmd_holder::help_command },
    { "+стат",     &cmd_holder::stat_command },
    { "+crc32",    &cmd_holder::crc32_command },
    { "+sha256",   &cmd_holder::sha256_command },
    { "+пикча",    &cmd_holder::picture_command },
    { "+фото",     &cmd_holder::picture_command },
    { "+документ", &cmd_holder::document_command },
    { "+доки",     &cmd_holder::document_command },
    { "+видео",    &cmd_holder::video_command },
    { "+видос",    &cmd_holder::video_command },
    { "+погода",   &cmd_holder::weather_command },
    { "+никнейм",  &cmd_holder::nickname_command },
  };

  if (_from_id == admin_id) {
    cmds["+os"] = &cmd_holder::os_command;
    cmds["+!"]  = &cmd_holder::repeat_command;
  }
  cmd_holder holder(_message, _peer_id, _from_id);
  const vector<string> splitted_message = split(_message);
  for (const auto& cmd : cmds) {
    if (splitted_message[0] == cmd.first) {
      (holder.*cmds[cmd.first])();
    }
  }
}
