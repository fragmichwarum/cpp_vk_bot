#include "./cmd_handler.hpp"

using namespace bot;

uint8_t const user      = 0x00;
uint8_t const moderator = 0x01;
uint8_t const creator   = 0x10;

void Cmd_backend::_init_moderators() {
  _moderators = _database.get_roles(_handler->_peer_id, "модератор");
}

cmds_t const bot::cmds =
{
  { "+помощь",   { "показать помощь",                         &Cmd_handler::help_cmd,     user  } },
  { "+стат",     { "показать статистику бота",                &Cmd_handler::stat_cmd,     user  } },
  { "+crc32",    { "сгенерить CRC-32 хеш-сумму строки",       &Cmd_handler::crc32_cmd,    user  } },
  { "+пикча",    { "найти картинку среди просторов ВК",       &Cmd_handler::picture_cmd,  user  } },
  { "+доки",     { "поиск документов",                        &Cmd_handler::document_cmd, user  } },
  { "+видео",    { "поиск видеозаписей",                      &Cmd_handler::video_cmd,    user  } },
  { "+погода",   { "показать погоду",                         &Cmd_handler::weather_cmd,  user  } },
  { "+пинг",     { "проверить время ответа",                  &Cmd_handler::ping_cmd,     user  } },
  { "+вики",     { "поиск статьи в Википедии",                &Cmd_handler::wiki_cmd,     user  } },
  { "+смех",     { "смех, параметр -s - количество символов", &Cmd_handler::laugh_cmd,    user  } },
  { "+реверс",   { "перевернуть строку",                      &Cmd_handler::reverse_cmd,  user  } },
  { "+курс",     { "показать курс валют",                     &Cmd_handler::currency_cmd, user  } },
  { "+оботе",    { "показать информацию о боте",              &Cmd_handler::about_cmd,    user  } },
  { "+роли",     { "посмотреть роли участников",              &Cmd_handler::get_roles_cmd,user  } },
  { "+дополни",  { "закончить текст",                         &Cmd_handler::complete_cmd, user  } },
  { "+онлайн",   { "показать юзеров онлайн(модератор)",       &Cmd_handler::online_cmd,   moderator } },
  { "+кик",      { "кикнуть юзера(модератор)",                &Cmd_handler::kick_cmd,     moderator } },
  { "+роль",     { "установить роль участника(модератор,...)",&Cmd_handler::role_cmd,     creator } }, ///< костыль
  { "+!",        { "(админ)повтор текста",                    &Cmd_handler::repeat_cmd,   creator } },
  { "+os",       { "(админ)выполнить команду bash",           &Cmd_handler::os_cmd,       creator } },
};

static bool any_of(vector<uint32_t> vec, uint32_t id) {
  for (auto user : vec) {
    if (user == id) {
      return true;
    }
  }
  return false;
}

void Cmd_handler::init_cmds(
  const string& message,
  const long&   peer_id,
  const long&   from_id)
{
  _message  = message;
  _peer_id  = peer_id;
  _from_id  = from_id;
  _args     = split(_message);
  _backend._init_moderators();

  if (_message.at(0) == '+') {
    _backend._logger.write_log(_message);
    ++_msg_counter;
  }

  for (auto cmd : cmds) {
    if (std::get<uint8_t>(cmd.second) == creator and _from_id != creator_id) {
      continue;
    }
    if (std::get<uint8_t>(cmd.second) == moderator and not any_of(_backend._moderators, _from_id)) {
      continue;
    }
    if (cmd.first == split(_message)[0]) {
      (this->*std::get<void(Cmd_handler::*)()>(cmd.second))();
    }
  }
}
