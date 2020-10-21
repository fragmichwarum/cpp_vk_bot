#include "cmd_handler.hpp"

cmd_handler::cmd_handler(nlohmann::json& json)
{
  auto response = json["updates"][0]["object"]["message"];
  _message = response["text"];
  _peer_id = response["peer_id"];
  _from_id = response["from_id"];

  Logger logger(logfile);
  if (_message[0] == '+') {
    logger.write_log(_message);
  }
}

void cmd_handler::init_cmds()
{
  map<string, void(cmd_holder::*)(void)> cmds;

  cmds = {
    { "+помощь",   &cmd_holder::help_cmd },
    { "+стат",     &cmd_holder::stat_cmd },
    { "+crc32",    &cmd_holder::crc32_cmd },
    { "+sha256",   &cmd_holder::sha256_cmd },
    { "+пикча",    &cmd_holder::picture_cmd },
    { "+фото",     &cmd_holder::picture_cmd },
    { "+документ", &cmd_holder::document_cmd },
    { "+доки",     &cmd_holder::document_cmd },
    { "+видео",    &cmd_holder::video_cmd },
    { "+видос",    &cmd_holder::video_cmd },
    { "+погода",   &cmd_holder::weather_cmd },
    { "+никнейм",  &cmd_holder::nickname_cmd },
  };

  if (_from_id == admin_id) {
    cmds["+os"] = &cmd_holder::os_cmd;
    cmds["+!"]  = &cmd_holder::repeat_cmd;
  }
  cmd_holder holder(_message, _peer_id, _from_id);
  const vector<string> splitted = split(_message);
  for (const auto& cmd : cmds) {
    if (splitted[0] == cmd.first) {
      (holder.*cmds[cmd.first])();
    }
  }
}
