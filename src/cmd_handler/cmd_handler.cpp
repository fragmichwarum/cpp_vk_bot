#include "../cmd_handler/cmd_handler.hpp"

using handler::Cmd_handler;

void Cmd_handler::init_cmds(
  const string& message,
  const long&   peer_id,
  const long&   from_id)
{
  _message          = message;
  _peer_id          = peer_id;
  _from_id          = from_id;
  _splitted_message = split(_message);
  _nickname         = _database.return_nickname(_from_id);

  unordered_map<string, void(Cmd_handler::*)(void)> cmds;

  cmds = {
    { "+помощь",     &Cmd_handler::help_cmd },
    { "+стат",       &Cmd_handler::stat_cmd },
    { "+crc32",      &Cmd_handler::crc32_cmd },
    { "+пикча",      &Cmd_handler::picture_cmd },
    { "+фото",       &Cmd_handler::picture_cmd },
    { "+документ",   &Cmd_handler::document_cmd },
    { "+доки",       &Cmd_handler::document_cmd },
    { "+видео",      &Cmd_handler::video_cmd },
    { "+видос",      &Cmd_handler::video_cmd },
    { "+погода",     &Cmd_handler::weather_cmd },
    { "+никнейм",    &Cmd_handler::nickname_cmd },
    { "+пинг",       &Cmd_handler::ping_cmd },
    { "+вики",       &Cmd_handler::wiki_cmd },
    { "+переводчик", &Cmd_handler::translate_cmd }
  };
  if (_from_id == admin_id) {
    cmds["+os"] = &Cmd_handler::os_cmd;
    cmds["+!"]  = &Cmd_handler::repeat_cmd;
  }
  const vector<string> splitted = split(_message);
  for (const auto& cmd : cmds) {
    if (cmd.first == splitted[0]) {
      (this->*cmds[cmd.first])();
    }
  }
}
