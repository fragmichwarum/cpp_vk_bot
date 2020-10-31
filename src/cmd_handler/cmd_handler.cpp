#include "./cmd_handler.hpp"

using namespace bot;

cmds_t const bot::cmds =
{
  { "+хелп",     { "показать помощь",                         &Cmd_handler::help_cmd,     USER  } },
  { "+помощь",   { "показать помощь",                         &Cmd_handler::help_cmd,     USER  } },
  { "+стат",     { "показать статистику бота",                &Cmd_handler::stat_cmd,     USER  } },
  { "+crc32",    { "сгенерить CRC-32 хеш-сумму строки",       &Cmd_handler::crc32_cmd,    USER  } },
  { "+пикча",    { "найти картинку среди просторов ВК",       &Cmd_handler::picture_cmd,  USER  } },
  { "+доки",     { "поиск документов",                        &Cmd_handler::document_cmd, USER  } },
  { "+видео",    { "поиск видеозаписей",                      &Cmd_handler::video_cmd,    USER  } },
  { "+погода",   { "показать погоду",                         &Cmd_handler::weather_cmd,  USER  } },
  { "+никнейм",  { "установить/удалить никнейм",              &Cmd_handler::nickname_cmd, USER  } },
  { "+пинг",     { "проверить время ответа",                  &Cmd_handler::ping_cmd,     USER  } },
  { "+вики",     { "поиск статьи в Википедии",                &Cmd_handler::wiki_cmd,     USER  } },
  { "+смех",     { "смех, параметр -s - количество символов", &Cmd_handler::laugh_cmd,    USER  } },
  { "+реверс",   { "перевернуть строку",                      &Cmd_handler::reverse_cmd,  USER  } },
  { "+курс",     { "показать курс валют",                     &Cmd_handler::currency_cmd, USER  } },
  { "+оботе",    { "показать информацию о боте",              &Cmd_handler::about_cmd,    USER  } },
  { "+онлайн",   { "показать юзеров онлайн(нужна админка)",   &Cmd_handler::online_cmd,   USER  } },
  { "+кик",      { "кикнуть юзера(нужна админка)",            &Cmd_handler::kick_cmd,     USER  } },
  { "+os",       { "(админ)выполнить команду bash",           &Cmd_handler::os_cmd,       ADMIN } },
  { "+!",        { "(админ)повтор текста",                    &Cmd_handler::repeat_cmd,   ADMIN } }
};

void Cmd_handler::init_cmds(
  const string& message,
  const long&   peer_id,
  const long&   from_id)
{
  _message           = message;
  _peer_id           = peer_id;
  _from_id           = from_id;
  _splitted_message  = split(_message);
  _nickname          = _backend._database.get(_from_id);

  if (_message.at(0) == '+') {
    _backend._logger.write_log(_message);
    ++_msg_counter;
  }

  for (auto cmd : cmds) {
    if (std::get<bool>(cmd.second) == ADMIN and _from_id != admin_id) {
      continue;
    }
    if (cmd.first == split(_message)[0]) {
      (this->*std::get<void(Cmd_handler::*)()>(cmd.second))();
    }
  }
}
