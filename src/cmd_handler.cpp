#include "cmd_handler.hpp"

using namespace bot;
using namespace cURL;

using nlohmann::json;

using std::string;
using std::vector;

static uint8_t const user      = 0x00;
static uint8_t const moderator = 0x01;
static uint8_t const creator   = 0x10;

cmds_t const bot::vk_cmds =
{
  { "+пинг",    { "проверить время ответа",                   &Cmds::ping_cmd,        user } },
  { "+помощь",  { "показать помощь",                          &Cmds::help_cmd,        user } },
  { "+стат",    { "показать статистику бота",                 &Cmds::stat_cmd,        user } },
  { "+crc32",   { "сгенерить CRC-32 хеш-сумму строки",        &Cmds::crc32_cmd,       user } },
  { "+пикча",   { "найти картинку среди просторов ВК",        &Cmds::picture_cmd,     user } },
  { "+видео",   { "поиск видеозаписей",                       &Cmds::video_cmd,       user } },
  { "+погода",  { "показать погоду",                          &Cmds::weather_cmd,     user } },
  { "+вики",    { "поиск статьи в Википедии",                 &Cmds::wiki_cmd,        user } },
  { "+смех",    { "смех, параметр -s - количество символов",  &Cmds::laugh_cmd,       user } },
  { "+оботе",   { "показать информацию о боте",               &Cmds::about_cmd,       user } },
  { "+курс",    { "показать курс валют",                      &Cmds::currency_cmd,    user } },
  { "+роли",    { "посмотреть роли участников",               &Cmds::get_roles_cmd,   user } },
  { "+дополни", { "закончить текст",                          &Cmds::complete_cmd,    user } },
  { "+гитхаб",  { "различная инфа о юзерах с GitHub",         &Cmds::github_info_cmd, user } },
  { "+трек",    { "получить ссылки на треки из Genius",       &Cmds::genius_cmd,      user } },
  { "+гугл",    { "редирект сложнейшено вопроса в гугл",      &Cmds::google_cmd,      user } },
  { "+доки",    { "поиск документов",                         &Cmds::document_cmd,    moderator } },
  { "+реверс",  { "перевернуть строку(модератор)",            &Cmds::reverse_cmd,     moderator } },
  { "+онлайн",  { "показать юзеров онлайн(модератор)",        &Cmds::online_cmd,      moderator } },
  { "+кик",     { "кикнуть юзера(модератор)",                 &Cmds::kick_cmd,        moderator } },
  { "+мут",     { "ограничить доступ к командам бота",        &Cmds::blacklist_cmd,   moderator } },
  { "+роль",    { "установить роль участника(модератор,...)", &Cmds::role_cmd,        creator } }, ///< костыль
  { "+!",       { "(админ) повтор текста",                    &Cmds::repeat_cmd,      creator } },
  { "+os",      { "(админ) выполнить команду bash",           &Cmds::os_cmd,          creator } },
  { "+офф",     { "(админ) завершить работу бота",            &Cmds::turn_off_cmd,    creator } }
};

void Vk_cmd_handler::init_roles(const long& peer_id) {
  moderators = database.get_by_role(peer_id, "модератор");
  blacklist  = database.get_by_role(peer_id, "мут");
}

bool exists(const json& object, const string& key) {
    return object.find(key) != object.end();
}

void Vk_cmd_handler::log(const string& message, const long& from_id) {
  msg_counter++;

  logger.print(
    LOGTYPE::LOG,
    message,
    std::to_string(from_id)
  );
  logger.write(
    LOGTYPE::LOG,
    message
  );
}

void Cmds::init_cmds(const json &update)
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

  string message;
  long peer_id;
  long from_id;
  if (update["type"] == "message_new") {
    message = update["object"]["message"][   "text"];
    peer_id = update["object"]["message"]["peer_id"];
    from_id = update["object"]["message"]["from_id"];

    if (message.at(0) == '+') {
      _vk_handler.log(message, from_id);
      _vk_handler.init_roles(peer_id);
    }
  }

  bool any_of_blacklisted =
    any_of(_vk_handler.blacklist.begin(),
           _vk_handler.blacklist.end  (), [&](long id){ return id == from_id; });

  bool any_of_moderators =
    any_of(_vk_handler.moderators.begin(),
           _vk_handler.moderators.end  (), [&](long id){ return id == from_id; });

  for (auto cmd : vk_cmds) {

    bool is_not_moderator = not any_of_moderators && std::get<access>(cmd.second) == moderator;

    bool is_not_creator = std::get<access>(cmd.second) == creator && from_id != creator_id;

    if (any_of_blacklisted) {
      continue;
    }

    if (is_not_moderator) {
      continue;
    }

    if (is_not_creator) {
      continue;
    }

    if (cmd.first == split(message)[0]) {
      _utils.message_send((this->*std::get<cmd_pointer>(cmd.second))({ message, peer_id }), peer_id);
    }
  }
}

void Cmds::stress_test(const string& peer_id) {
  string vkurl = append_vkurl("messages.send");
  params body;

  vector<string> cmd = {
    "+crc32",
    "+пикча",
    "+видео",
    "+доки",
    "+погода",
//    "+дополни",
//    "+реверс",
    "+вики",
  };
  vector<string> words = {
    "понятие",
    "в",
    "Москва",
    "раз"
  };

  body["message"] = cmd[rand() % cmd.size()] + ' ' + words[rand() % words.size()];
  body["peer_id"] = peer_id;
  body["disable_mentions"] = "1";
  body["access_token"] = stress_test_token;
  body["random_id"] = "0";
  body["v"] = api_version;
  request(vkurl, body);
}
