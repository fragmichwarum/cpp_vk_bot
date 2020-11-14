#include "cmd_handler.hpp"

using namespace bot::util;
using bot::cmds_t;
using bot::Cmd_handler;

using std::string;
using std::vector;
using nlohmann::json;

cmds_t const bot::vk_cmds =
{
  { "+пинг",    { "проверить время ответа",                   &Cmd_handler::ping_cmd,        user } },
  { "+помощь",  { "показать помощь",                          &Cmd_handler::help_cmd,        user } },
  { "+стат",    { "показать статистику бота",                 &Cmd_handler::stat_cmd,        user } },
  { "+crc32",   { "сгенерить CRC-32 хеш-сумму строки",        &Cmd_handler::crc32_cmd,       user } },
  { "+пикча",   { "найти картинку среди просторов ВК",        &Cmd_handler::picture_cmd,     user } },
  { "+видео",   { "поиск видеозаписей",                       &Cmd_handler::video_cmd,       user } },
  { "+погода",  { "показать погоду",                          &Cmd_handler::weather_cmd,     user } },
  { "+вики",    { "поиск статьи в Википедии",                 &Cmd_handler::wiki_cmd,        user } },
  { "+смех",    { "смех, параметр -s - количество символов",  &Cmd_handler::laugh_cmd,       user } },
  { "+оботе",   { "показать информацию о боте",               &Cmd_handler::about_cmd,       user } },
  { "+курс",    { "показать курс валют",                      &Cmd_handler::currency_cmd,    user } },
  { "+роли",    { "посмотреть роли участников",               &Cmd_handler::get_roles_cmd,   user } },
  { "+дополни", { "закончить текст",                          &Cmd_handler::complete_cmd,    user } },
  { "+гитхаб",  { "различная инфа о юзерах с GitHub",         &Cmd_handler::github_info_cmd, user } },
  { "+трек",    { "получить ссылки на треки из Genius",       &Cmd_handler::genius_cmd,      user } },
  { "+гугл",    { "редирект сложнейшено вопроса в гугл",      &Cmd_handler::google_cmd,      user } },
  { "+доки",    { "поиск документов",                         &Cmd_handler::document_cmd,    user } },
  { "+реверс",  { "перевернуть строку",                       &Cmd_handler::reverse_cmd,     moderator } },
  { "+онлайн",  { "показать юзеров онлайн",                   &Cmd_handler::online_cmd,      moderator } },
  { "+кик",     { "кикнуть юзера",                            &Cmd_handler::kick_cmd,        moderator } },
  { "+мут",     { "ограничить доступ к боту",                 &Cmd_handler::blacklist_cmd,   moderator } },
  { "+роль",    { "установить роль участника",                &Cmd_handler::role_cmd,        creator } }, ///< костыль
  { "+!",       { "повтор текста",                            &Cmd_handler::repeat_cmd,      creator } },
  { "+os",      { "выполнить команду bash",                   &Cmd_handler::os_cmd,          creator } },
  { "+офф",     { "завершить работу бота",                    &Cmd_handler::turn_off_cmd,    creator } }
};

static bool exists(const json& object, const string& key) {
  return object.find(key) != object.end();
}

void Cmd_handler::_log(const string& message, const long& from_id) {
  _logger.print(LOGTYPE::LOG, message, std::to_string(from_id));
  _logger.write(LOGTYPE::LOG, message);
}

void Cmd_handler::init_cmds(const nlohmann::json& update)
{
  if (update["type"] == "wall_post_new") {
    new_post_event(update["object"]);
    return;
  }

  if (update["type"] != "message_new") {
    return;
  }

  if (update["object"]["message"]["text"] == "") {
    return;
  }

  if (exists(update["object"]["message"],"reply_message")) {
    _reply = update["object"]["message"]["reply_message"];
  }

  vector<long> moderators;
  vector<long> blacklist;
  string message;
  long peer_id   = 0;
  long from_id   = 0;
  if (json event = update["object"]["message"]; update["type"] == "message_new") {
    message = event[   "text"];
    peer_id = event["peer_id"];
    from_id = event["from_id"];

    if (message.at(0) == '+') {
      ++_msg_counter;
      _log(message, from_id);
      moderators = _database.get_by_role(peer_id, "модератор");
      blacklist  = _database.get_by_role(peer_id, "мут");
    }
  }

  bool any_of_blacklisted =
      any_of(blacklist.begin(),
             blacklist.end  (), [&](long id){ return id == from_id; });

    bool any_of_moderators =
      any_of(moderators.begin(),
             moderators.end  (), [&](long id){ return id == from_id; });

  for (auto cmd : vk_cmds) {
    bool is_not_moderator = not any_of_moderators && std::get<_Access>(cmd.second) == moderator;

    bool is_not_creator = std::get<_Access>(cmd.second) == creator && from_id != creator_id;

    if (any_of_blacklisted || is_not_moderator || is_not_creator) {
      continue;
    }
    if (cmd.first == split(message)[0]) {
      string text = (this->*std::get<_Cmd_pointer>(cmd.second))({ message, peer_id });
      if (text != "") {
        _api.send_message(text, peer_id);
      }
    }
  }
}
