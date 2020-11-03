#include "./cmd_handler.hpp"

using namespace bot;

uint8_t const user      = 0x00;
uint8_t const moderator = 0x01;
uint8_t const creator   = 0x10;

vector<string> bot::_words_from_file(const string& filename) {
  std::ifstream file(filename);
  vector<string> result;
  for (string line; std::getline(file, line); ) {
    result.push_back(line);
  }
  return result;
}

string bot::_cyrillic_to_lower(const string& text) {
  wstring wide_string = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().from_bytes(text);
  std::transform(
    wide_string.begin(),
    wide_string.end(),
    wide_string.begin(),
    std::bind2nd(std::ptr_fun(&std::tolower<wchar_t>), std::locale("ru_RU.UTF-8"))
  );
  return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(wide_string);
}

void Cmd_backend::_init_roles() {
  _moderators = _database.get_by_role(_handler->_peer_id, "модератор");
  _blacklist = _database.get_by_role(_handler->_peer_id, "мут");
}

void Cmd_backend::_init_words_blacklist() {
  _words_blacklist = _words_from_file(word_blacklist);
}

cmds_t const bot::cmds =
{
  { "+помощь",   { "показать помощь",                         &Cmd_handler::help_cmd,      user  } },
  { "+стат",     { "показать статистику бота",                &Cmd_handler::stat_cmd,      user  } },
  { "+crc32",    { "сгенерить CRC-32 хеш-сумму строки",       &Cmd_handler::crc32_cmd,     user  } },
  { "+пикча",    { "найти картинку среди просторов ВК",       &Cmd_handler::picture_cmd,   user  } },
  { "+доки",     { "поиск документов",                        &Cmd_handler::document_cmd,  user  } },
  { "+видео",    { "поиск видеозаписей",                      &Cmd_handler::video_cmd,     user  } },
  { "+погода",   { "показать погоду",                         &Cmd_handler::weather_cmd,   user  } },
  { "+пинг",     { "проверить время ответа",                  &Cmd_handler::ping_cmd,      user  } },
  { "+вики",     { "поиск статьи в Википедии",                &Cmd_handler::wiki_cmd,      user  } },
  { "+смех",     { "смех, параметр -s - количество символов", &Cmd_handler::laugh_cmd,     user  } },
  { "+реверс",   { "перевернуть строку",                      &Cmd_handler::reverse_cmd,   user  } },
  { "+курс",     { "показать курс валют",                     &Cmd_handler::currency_cmd,  user  } },
  { "+оботе",    { "показать информацию о боте",              &Cmd_handler::about_cmd,     user  } },
  { "+роли",     { "посмотреть роли участников",              &Cmd_handler::get_roles_cmd, user  } },
  { "+дополни",  { "закончить текст",                         &Cmd_handler::complete_cmd,  user  } },
  { "+онлайн",   { "показать юзеров онлайн(модератор)",       &Cmd_handler::online_cmd,    moderator } },
  { "+кик",      { "кикнуть юзера(модератор)",                &Cmd_handler::kick_cmd,      moderator } },
  { "+мут",      { "ограничить доступ к командам бота",       &Cmd_handler::blacklist_cmd, moderator } },
  { "+роль",     { "установить роль участника(модератор,...)",&Cmd_handler::role_cmd,      creator } }, ///< костыль
  { "+!",        { "(админ)повтор текста",                    &Cmd_handler::repeat_cmd,    creator } },
  { "+os",       { "(админ)выполнить команду bash",           &Cmd_handler::os_cmd,        creator } },
};

template <class T>
bool bot::_any_of(vector<T>& vec, T id) {
  return std::find(vec.begin(), vec.end(), id) != vec.end();
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
  _backend._init_roles();
  _backend._init_words_blacklist();

  if (_message.at(0) == '+') {
    _backend._logger.write_log(_message);
    ++_msg_counter;
  }

  if (_any_of<string>(_backend._words_blacklist, _cyrillic_to_lower(_message))) {
    return;
  }

  for (auto cmd : cmds) {
    if (std::get<uint8_t>(cmd.second) == creator and _from_id != creator_id) {
      continue;
    }
    if (std::get<uint8_t>(cmd.second) == moderator and not _any_of<uint32_t>(_backend._moderators, _from_id)) {
      continue;
    }
    if (_any_of<uint32_t>(_backend._blacklist, _from_id)) {
      continue;
    }
    if (cmd.first == split(_message)[0]) {
      (this->*std::get<void(Cmd_handler::*)()>(cmd.second))();
    }
  }
}
