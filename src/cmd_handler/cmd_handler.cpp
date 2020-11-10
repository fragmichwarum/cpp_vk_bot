#include "./cmd_handler.hpp"

using namespace bot;
using namespace cURL;

using nlohmann::json;

using std::string;
using std::vector;

uint8_t const user      = 0x00;
uint8_t const moderator = 0x01;
uint8_t const creator   = 0x10;

cmds_t const bot::vk_cmds =
{
  { "+помощь",  { "показать помощь",                          &Cmds::help_cmd,        user } },
  { "+стат",    { "показать статистику бота",                 &Cmds::stat_cmd,        user } },
  { "+crc32",   { "сгенерить CRC-32 хеш-сумму строки",        &Cmds::crc32_cmd,       user } },
  { "+пикча",   { "найти картинку среди просторов ВК",        &Cmds::picture_cmd,     user } },
  { "+видео",   { "поиск видеозаписей",                       &Cmds::video_cmd,       user } },
  { "+погода",  { "показать погоду",                          &Cmds::weather_cmd,     user } },
  { "+пинг",    { "проверить время ответа",                   &Cmds::ping_cmd,        user } },
  { "+вики",    { "поиск статьи в Википедии",                 &Cmds::wiki_cmd,        user } },
  { "+смех",    { "смех, параметр -s - количество символов",  &Cmds::laugh_cmd,       user } },
  { "+инфо",    { "показать информацию о боте",               &Cmds::about_cmd,       user } },
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
  { "+запрети", { "добавить слово в список запрещённых",      &Cmds::forbid_word_cmd, moderator } },
  { "+роль",    { "установить роль участника(модератор,...)", &Cmds::role_cmd,        creator } }, ///< костыль
  { "+!",       { "(админ) повтор текста",                    &Cmds::repeat_cmd,      creator } },
  { "+os",      { "(админ) выполнить команду bash",           &Cmds::os_cmd,          creator } },
  { "+офф",     { "(админ) завершить работу бота",            &Cmds::turn_off_cmd,    creator } }
};

vector<string> bot::words_from_file(const string& filename) {
  std::ifstream file(filename);
  vector<string> words;
  string word;
  while (file >> word) {
    words.push_back(word);
  }
  return words;
}

string bot::utf8_to_lower(const string& text) {
  using std::wstring;
  using std::wstring_convert;
  using std::codecvt_utf8;
  using std::bind2nd;
  using std::tolower;

  wstring wide_string = wstring_convert<codecvt_utf8<wchar_t>, wchar_t>().from_bytes(text);
  std::transform(
    wide_string.begin(), wide_string.end(), wide_string.begin(),
    bind2nd(std::ptr_fun(&tolower<wchar_t>), std::locale(""))
  );
  return wstring_convert<codecvt_utf8<wchar_t>, wchar_t>().to_bytes(wide_string);
}

void Vk_cmd_handler::init_roles(const long& peer_id) {
  moderators = database.get_by_role(peer_id, "модератор");
  blacklist  = database.get_by_role(peer_id, "мут");
}

void Vk_cmd_handler::init_conservations() {
  conservations = database.get_peer_ids();
}

void Vk_cmd_handler::init_words_blacklist() {
  words_blacklist = words_from_file(word_blacklist);
}

bool bot::exists(const json& object, const string& key) {
    return object.find(key) != object.end();
}

void Vk_cmd_handler::log(const string& message) {
  if (message.at(0) == '+') {
    logger.write_log(message);
    msg_counter++;
  }
}

void Cmds::init_cmds(const json &update)
{
  if (exists(update["object"]["message"],"reply_message")) {
    _reply = update["object"]["message"]["reply_message"];
  }

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

  string message;
  long peer_id;
  long from_id;
  if (update["type"] == "message_new") {
    message = update["object"]["message"][   "text"];
    peer_id = update["object"]["message"]["peer_id"];
    from_id = update["object"]["message"]["from_id"];

    _vk_handler.log(message);
    _vk_handler.init_roles(peer_id);
  }

  for (const auto& arg : split(message)) {

    if (std::any_of(
          _vk_handler.words_blacklist.begin(),
          _vk_handler.words_blacklist.end(),
          [&](string word){ return utf8_to_lower(arg).find(word) != string::npos; }))
    {
      return;
    }
  }


  bool any_of_blacklisted =
    any_of(_vk_handler.blacklist.begin(),
           _vk_handler.blacklist.end  (), [&](long id){ return id == from_id; });

  bool any_of_moderators =
    any_of(_vk_handler.moderators.begin(),
           _vk_handler.moderators.end  (), [&](long id){ return id == from_id; });

  for (auto cmd : vk_cmds) {

    bool is_not_moderator = not any_of_moderators and std::get<access>(cmd.second) == moderator;

    bool is_not_creator = std::get<access>(cmd.second) == creator and from_id != creator_id;

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
      _vk_handler.message_send((this->*std::get<cmd_pointer>(cmd.second))({ message, peer_id }), peer_id);
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
  vector<string> words = words_from_file("/home/machen/words.txt");

  body["message"] = cmd[rand() % cmd.size()] + ' ' + words[rand() % words.size()];
  body["peer_id"] = peer_id;
  body["disable_mentions"] = "1";
  body["access_token"] = stress_test_token;
  body["random_id"] = "0";
  body["v"] = api_version;
  request(vkurl, body);
}
