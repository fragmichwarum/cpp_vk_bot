#include "cmd_handler.hpp"
#include "lib/crc32.hpp"

using namespace bot;

Vk_cmd_handler::Vk_cmd_handler(Cmds& handler) {
  _handler = &handler;
  init_words_blacklist();
}

Vk_cmd_handler::~Vk_cmd_handler() {
  delete _handler;
  _handler = nullptr;
}

string Vk_cmd_handler::get_cmd_body() {
  string cmd_body;
  bool first_word = false;
  for (string arg : split(_handler->_message)) {
    if (not first_word) {
      first_word = true;
      continue;
    }
    cmd_body += arg + " ";
  }
  return cmd_body;
}

void Vk_cmd_handler::message_send(const string& text) {
  string vkurl = append_vkurl("messages.send");
  params body;
  append_vkparams(body);

  body["message"] = text;
  body["peer_id"] = to_string(_handler->_peer_id);
  body["disable_mentions"] = "1";
  body["random_id"] = "0";
  request(vkurl, body);
}

string Vk_cmd_handler::media_search(const string& method) {
  json parsed =
    json::parse(request(append_vkurl(method),
      {{ "q",            get_cmd_body() },
       { "access_token", user_token      },
       { "count",        "50"            },
       { "v",            api_version     }}));

  long size = parsed["response"]["items"].size();
  if (size == 0) {
    message_send(media_not_found(attachment_type(method)));
    return "";
  }

  string attachments;
  long index;

  for (int i = 0; i < size; i++) {
    if (i == 10) {
      break;
    }
    index = rand() % size;
    json media = parsed["response"]["items"][index];
    string owner_id = to_string(media["owner_id"].get<long>());
    string id       = to_string(media["id"].get<long>());
    attachments    += attachment_type(method) + owner_id + '_' + id + ',';
  }
  params attachment_body = {{"attachment", attachments },
                            {"peer_id", to_string(_handler->_peer_id)}};
  append_vkparams(attachment_body);
  string vkurl = append_vkurl("messages.send");
  return request(vkurl, attachment_body);
}

string Vk_cmd_handler::attachment_type(const string& method) {
  if (method == "photos.search") {
    return "photo";
  } else if (method == "video.search") {
    return "video";
  } else if (method == "docs.search") {
    return "doc";
  } else {
    return "";
  }
}

string Vk_cmd_handler::media_not_found(const string& type) {
  if (type == "photo") {
    return "Не найдено фотографий!";
  } else if (type == "video") {
    return "Не найдено видеозаписей!";
  } else if (type == "doc") {
    return "Не найдено такого документа!";
  }
  return "";
}

string Vk_cmd_handler::empty_args() {
  return "Задана пустая строка.";
}

string Cmds::document_cmd() {
  if (_message == "+доки") {
    return _vk_handler.empty_args();
  }
  _vk_handler.media_search("docs.search");
  return "";
}

string Cmds::picture_cmd() {
  if (_message == "+пикча") {
    return _vk_handler.empty_args();
  }
  _vk_handler.media_search("photos.search");
  return "";
}

string Cmds::video_cmd() {
  if (_message == "+видео") {
    return _vk_handler.empty_args();
  }
  _vk_handler.media_search("video.search");
  return "";
}

string Cmds::weather_cmd() {
  if (_message == "+погода") {
    return _vk_handler.empty_args();
  }
  string open_weather_url = "http://api.openweathermap.org/data/2.5/weather?";
  json parsed =
      json::parse(request(open_weather_url,
       {{ "lang", "ru"                                },
        { "units", "metric"                           },
        { "APPID", "ef23e5397af13d705cfb244b33d04561" },
        { "q",     _args[1]                           }}));
  if (parsed["weather"].is_null()) {
    return "Нет такого города.";
  }
  string description = parsed["weather"][0]["description"];
  int temp           = parsed["main"]["temp"];
  int feels_like     = parsed["main"]["feels_like"];
  int humidity       = parsed["main"]["humidity"];
  string city_name   = parsed["name"];
  string weather =
    "Сейчас в  " + city_name + " " + to_string(temp) +
    "°C, " + description + "\nОщущается как " +
    to_string(feels_like) + "°C\nВлажность: " +
    to_string(humidity) + "%";

  return weather;
}

json wiki_search(const string& wiki_url, const params& text) {
  return json::parse(request(wiki_url, text));
}

string Cmds::wiki_cmd() {
  if (_message == "+вики") {
    return _vk_handler.empty_args();
  }
  string wiki_url = "https://ru.wikipedia.org/w/api.php?";
  string page;
  json parsed;

  try {
    parsed =
      wiki_search(wiki_url,
       {{"titles",      _vk_handler.get_cmd_body() },
        {"action",      "query"    },
        {"format",      "json"     },
        {"prop",        "extracts" },
        {"exintro",     ""         },
        {"explaintext", ""         },
        {"redirects",   "1"        }});
    for (auto i : parsed["query"]["pages"].get<json::object_t>()) {
      page = i.first;
     break;
    }

    if (page != "-1") {
      return parsed["query"]["pages"][page]["extract"];
    }
  } catch(nlohmann::json::parse_error&) {

  } catch (nlohmann::json::type_error&) {

  }

  try {
    parsed = wiki_search(wiki_url,
     {{"action", "query"},
      {"list", "search"},
      {"format","json"},
      {"srsearch", curl_easy_escape(NULL, _vk_handler.get_cmd_body().c_str(), _vk_handler.get_cmd_body().length())}});
    if (parsed["query"]["search"].size() == 0) {
      _vk_handler.message_send("Такой статьи не найдено.");
      return "Такой статьи не найдено.";
    }
    return parsed["query"]["search"][0]["snippet"];

  } catch(nlohmann::json::parse_error&) {

  } catch (nlohmann::json::type_error&) {

  }
  return "";
}

namespace {
string laugh(size_t len = 10) {
  srand(time(NULL));
  const vector<string> alphabet = {
    "а", "А",
    "х", "Х",
    "ж", "Ж",
    "п", "П",
    "ы", "Ы",
    "ъ", "Ъ"
  };
  string result;
  for (size_t i = 0; i < len; i++) {
    result += alphabet[rand() % alphabet.size()];
  }
  return result;
}
} //namespace

string Cmds::laugh_cmd() {
  if (_args.size() == 1) {
    return laugh();
  }

  if (_args[1] != "-s" and _args.size() > 1) {
    return "Неверный параметр.";
  }

  if (_args[1] == "-s" and _args.size() == 2) {
    return "Введи количество символов.";
  }

  if (_args[1] != "-s" and _args.size() >= 3) {
    return "";
  }

  if (_args[2][0] == '-') {
    return "Отрицательное количество символов, серьёзно?";
  }

  if (not std::all_of(_args[2].begin(), _args[2].end(), ::isdigit)) {
    return "Аргумент не является целым числом.";
  }

  if (_args[2].size() >= 6) {
    return "Слишком большое число.";
  }

  short length = std::stoi(_args[2]);

  if (length > 500) {
    return "Слишком большое число.";
  }

  if (length <= 0) {
    return "Длина строки меньше или равно 0?";
  }

  return laugh(length);
}

string Cmds::reverse_cmd() {
  setlocale(LC_CTYPE, "");
  if (_message == "+реверс") {
    return _vk_handler.empty_args();
  }
  return reverse(_vk_handler.get_cmd_body().c_str());
}

string Cmds::currency_cmd() {
  json parsed = json::parse(request("https://www.cbr-xml-daily.ru/daily_json.js", {}));
  string result;
  vector<string> currency_list = {
    "GBP",
    "BYN",
    "USD",
    "EUR",
    "KZT",
    "PLN",
    "UAH",
    "JPY"
  };
  result += "Курс валют:\n";
  for (string currency : currency_list) {
    result += to_string(parsed["Valute"][currency]["Nominal"].get<long>());
    result += ' ';
    result += parsed["Valute"][currency]["Name"];
    result += " -> ";
    result += to_string(parsed["Valute"][currency]["Value"].get<double>()) + "₽\n";
  }
  return result;
}

string Cmds::help_cmd() {
  string help_info = "Список команд:\n";
  for (auto cmd : vk_cmds) {
    help_info += cmd.first + " - " + std::get<string>(cmd.second) + '\n';
  }
  return help_info;
}

string Cmds::about_cmd() {
  return
    "C++ bot,\n"
    "сурсы лежат тут: https://github.com/oxfffffe/cpp_vk_bot\n"
    "бота создал: @jijijijijijijijijijijijji (он)\n"
    "версия VK API: " + api_version + '\n' +
    "собран: " + _vk_handler.build_time;
}

string Cmds::crc32_cmd() {
  if (_message == "+crc32") {
    return _vk_handler.empty_args();
  }
  unsigned long crc32 = crc32gen(_vk_handler.get_cmd_body().c_str());
  stringstream stream;
  stream << "0x" << std::hex << crc32;
  return stream.str();
}

static string os_exec(string const& cmd) {
  string result;
  array<char, 128> buffer;
  unique_ptr<FILE, decltype (&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

string Cmds::os_cmd() {
  string cmd = _vk_handler.get_cmd_body();
  return os_exec(cmd);
}

string Cmds::repeat_cmd() {
  return _vk_handler.get_cmd_body();
}

static string lineparse(const string& line) {
  string result;
  for (char c : line) {
    if (isdigit(c)) {
      result += c;
    }
  }
  return result;
}

static string procinfo(const string& filename, const string& param) {
  FILE* file = fopen(filename.c_str(), "r");
  string result;
  array<char, 128> line;

  while (fgets(line.data(), 128, file) != NULL) {
    if (strncmp(line.data(), param.c_str(), param.size()) == 0) {
      return lineparse(line.data());
    }
  }
  fclose(file);
  return "";
}

string Cmds::stat_cmd() {
  return
    "Всего памяти: "      + procinfo("/proc/meminfo", "MemTotal:") + "KiB.\n"
    "Использовано ОЗУ: "  + procinfo("/proc/self/status", "VmRSS:") + "KiB.\n"
    "Потоков занято: "    + procinfo("/proc/self/status", "Threads:") + '\n' +
    "Аптайм: "            + os_exec("ps -eo lstart,etime,args | grep vk | awk '{print $6}' | head -1") +
    "Команд обработано: " + to_string(_msg_counter);
}

string Cmds::ping_cmd() {
  clock_t now = clock();
  request(append_vkurl("groups.getLongPollServer"), {
            { "group_id",     group_id     },
            { "access_token", access_token },
            { "v",            api_version  }
         });
  return "Сообщение обработано за: " +
            to_string((float)(clock() - now)/CLOCKS_PER_SEC * 10000) +
            " ms.";
}

string Cmds::online_cmd() {
  uint16_t access_err = 917;
  string vkurl = append_vkurl("messages.getConversationMembers");
  params body  = {{"fields", "online"}, {"peer_id", to_string(_peer_id)}};
  append_vkparams(body);

  json parsed = json::parse(request(vkurl, body));
  if (not parsed["error"].is_null() and
          parsed["error"]["error_code"] == access_err)
  {
    return "Упс, кажется у бота нет админки.";
  }
  string people = "Список людей онлайн:\n";
  for (auto profile : parsed["response"]["profiles"]) {
    if (profile["online"] == 1) {
      people += "@id" + to_string(profile["id"].get<long>()) + "(";
      people += profile["first_name"].get<string>() + " " + profile["last_name"].get<string>() + ")\n";
    }
  }
  return people;
}

string Vk_cmd_handler::ret_id(const string& id) {
  long id_len = 9;
  return id.substr(3, id_len);
}

string Cmds::kick_cmd() {
  if (_message == "+кик") {
    return _vk_handler.empty_args();
  }
  params body = {{"chat_id", to_string(_peer_id - 2000000000)},
                 {"user_id", _vk_handler.ret_id(_vk_handler.get_cmd_body())}};
  append_vkparams(body);
  json response = json::parse(request(append_vkurl("messages.removeChatUser"), body));
  if (not response["error"].is_null() and
          response["error"]["error_code"] == 100)
  {
    return "Что-то пошло не так.";
  }
  return "";
}

string Cmds::role_cmd() {
  if (_message == "+роль") {
    return _vk_handler.empty_args();
  }
  if (_args.size() == 3) {
    long id = std::stol(_vk_handler.ret_id(_args[1]));
    _vk_handler.database.insert_role(id, _peer_id, _args[2]);
    return "Роль успешно установлена.";
  } else {
    return "Что-то пошло не так, проверь правильность аргументов.";
  }
}
string Cmds::get_roles_cmd() {
  if (_args.size() == 1) {
    return "Что-то пошло не так, проверь правильность аргументов.";
  }
  vector<long> roles = _vk_handler.database.get_by_role(_peer_id, _args[1]);
  if (roles.size() == 0) {
    return "В этом чате нет участников с данной ролью.";
  }
  string persons;
  bool is_comma = false;
  for (uint32_t person : roles) {
    if (is_comma) {
      persons += ',';
    }
    is_comma = true;
    persons += to_string(person);
  }
  json parsed =
    json::parse(request(append_vkurl("users.get"),
     {{"user_ids",     persons           },
      {"access_token", user_token        },
      {"v",            api_version      }}));
  string moderators = "Список участников с данной ролью:\n";
  for (uint8_t i = 0; i < roles.size(); i++) {
    moderators += "@id";
    moderators += to_string(roles.at(i));
    moderators += " (";
    moderators += parsed["response"][i]["first_name"];
    moderators += " ";
    moderators += parsed["response"][i]["last_name"];
    moderators += ")";
    moderators += '\n';
  }
  return moderators;
}

string Cmds::blacklist_cmd() {
  if (_message == "+мут") {
    return _vk_handler.empty_args();
  }
  if (_args.size() == 2) {
    long id = std::stol(_vk_handler.ret_id(_args[1]));
    _vk_handler.database.insert_role(id, _peer_id, "мут");
    return "Готово.";
  } else {
    return "Что-то пошло не так, проверь правильность аргументов.";
  }
}

string Cmds::complete_cmd() {
  if (_message == "+дополни") {
    return _vk_handler.empty_args();
  }
  string AIurl = "https://pelevin.gpt.dobro.ai/generate/";
  string body = _vk_handler.get_cmd_body();
  json parsed =
    json::parse(requestdata(
                  AIurl, to_json({{"prompt", body}, {"length", "50"}})));

  return body + parsed["replies"][0].get<string>();
}

string Cmds::forbid_word_cmd() {
  if (_message == "+запрети") {
    return _vk_handler.empty_args();
  }
  ofstream _log (word_blacklist, std::ios::app);
  _log << _args[1] << "\n";
  _log.close();
  _vk_handler.init_words_blacklist();
  return "Слово было запрещено.";
}

struct non_alpha {
  bool operator()(char c) {
    return not (isdigit(c) or isalpha(c));
  }
};

bool is_latin(const string& text) {
  return std::find_if(text.begin(), text.end(), non_alpha()) == text.end();
}

string github_get_user_repo(const string& user, const string& repo_name) {
  std::cout << user << std::endl;
  json repos = json::parse(request("https://api.github.com/users/" + user + "/repos", {}));
  if (repos.is_object()) {
    return "Упс, кажется такого юзера нет.";
  }
  string founded_repo = "Информация о репозитории ";
  bool is_found = false;
  for (json repo : repos) {
    if (repo["name"] == repo_name) {
      is_found = true;
      founded_repo += repo["name"];
      founded_repo += ":\n";
      founded_repo += "Описание: ";
      founded_repo += not repo["description"].is_null() ? repo["description"] : "отсутствует";
      founded_repo += "\nЗвёзд: ";
      founded_repo += to_string(repo["stargazers_count"].get<long>());
      founded_repo += "\nДоминирующий язык: ";
      founded_repo += not repo["language"].is_null() ? repo["language"] : "отсутствует";
      founded_repo += "\nСсылка: ";
      founded_repo += repo["owner"]["html_url"];
      founded_repo += "\nЕсли хочешь склонить: ";
      founded_repo += repo["clone_url"];
      founded_repo += "\nФорк: ";
      founded_repo += repo["fork"].get<bool>() ? "да\n\n" : "нет\n\n";
      break;
    }
  }
  if (not is_found) {
    return "Упс, кажется у этого юзера нет такого репозитория.";
  }
  return founded_repo;
}

string github_get_user_repos(const string& user) {
  json parsed = json::parse(request("https://api.github.com/users/" + user + "/repos", {}));
  if (parsed.is_object()) {
    return "Упс, кажется такого юзера нет.";
  }
  string user_repos = "Информация о репозиториях:\n";
  for (uint8_t i = 0; i < parsed.size(); i++) {
    if (i == 5) {
      break;
    }
    user_repos += parsed[i]["name"];
    user_repos += ":\n";
    user_repos += "Описание: ";
    user_repos += not parsed[i]["description"].is_null() ? parsed[i]["description"] : "отсутствует";
    user_repos += "\nЗвёзд: ";
    user_repos += to_string(parsed[i]["stargazers_count"].get<long>());
    user_repos += "\nДоминирующий язык: ";
    user_repos += not parsed[i]["language"].is_null() ? parsed[i]["language"] : "отсутствует";
    user_repos += "\nСсылка: ";
    user_repos += parsed[i]["owner"]["html_url"];
    user_repos += "\nЕсли хочешь склонить: ";
    user_repos += parsed[i]["clone_url"];
    user_repos += "\nФорк: ";
    user_repos += parsed[i]["fork"].get<bool>() ? "да\n\n" : "нет\n\n";
  }
  return user_repos;
}

string github_get_user_info(const string& user) {
  json parsed = json::parse(request("https://api.github.com/users/" + user, {}));
  if (not parsed["message"].is_null()) {
    return "Упс, кажется такого юзера нет.";
  }
  string github_user = parsed["html_url"];
  github_user += ":\n";
  github_user += "Публичных репозиториев: ";
  github_user += to_string(parsed["public_repos"].get<long>());
  github_user += "\nПодписчиков: ";
  github_user += to_string(parsed["followers"].get<long>());
  github_user += "\nПодписок: ";
  github_user += to_string(parsed["following"].get<long>());
  return github_user;
}

string github_get_user_followers(const string& user) {
  json parsed = json::parse(request("https://api.github.com/users/" + user + "/followers", {}));
  if (parsed.is_object()) {
    return "Упс, кажется такого юзера нет.";
  }
  string github_followers = "Список подпсчиков пользователя:\n";
  for (auto follower : parsed) {
    github_followers += follower["login"];
    github_followers += " - ";
    github_followers += follower["html_url"];
    github_followers += "\n";
  }
  return github_followers;
}

string Cmds::github_info_cmd() {
  if (_message == "+гитхаб") {
    return _vk_handler.empty_args();
  }

  string option = _args[1];
  string user;

  if (_args.size() == 3 or _args.size() == 4) {
    user = _args[2];
  }

  if (not is_latin(user)) {
    return "Недопустимые символы.";
  }

  vector<string> options = { "-репо", "-подписчики", "-юзер", "-помощь" };

  if (not any_of(options, option)) {
    return "Неверный параметр. Напиши \"+гитхаб -помощь\", чтобы узнать о "
           "параметрах.";
  }

  if (_args.size() == 4 and option == "-репо") {
    return github_get_user_repo(user, _args[3]);
  }

  if (_args.size() == 3 and option == "-репо") {
    return github_get_user_repos(user);
  }

  if (_args.size() == 3 and option == "-юзер") {
    return github_get_user_info(user);
  }

  if (_args.size() == 3 and option == "-подписчики") {
    return github_get_user_followers(user);
  }

  if (_args.size() == 2 and option == "-помощь") {
    return "+гитхаб -репо <никнейм> - получить информацию о репозиториях(до 5 шт.) пользователя,\n"
           "+гитхаб -репо <никнейм> <репозиторий> - получить информацию о конкретном пользователя,\n"
           "+гитхаб -подписчики <никнейм> - получить список людей, подписанных на <никнейм>,\n"
           "+гитхаб -юзер <никнейм> - получить информацию о аккаунте GitHub.";
  }
  return "";
}

string Cmds::genius_cmd() {
  if (_message == "+трек") {
    return _vk_handler.empty_args();
  }
  string genius_token = "JSgH4gUYSn3S2C6Wd4BUhGuV1FWaKSET9DQVl-HBqlqeQ3isoW5bXSllR90VKvQF";
  json songs =
    json::parse(request("https://api.genius.com/search?",
     {{"q",            _vk_handler.get_cmd_body() },
      {"access_token", genius_token               }}));

  if (songs["response"]["hits"].size() == 0) {
    return "Кажется такого исполнителя нет.";
  }
  string songs_message;
  long resp_size = 0;
  for (json song : songs["response"]["hits"]) {
    if (resp_size++ == 10) {
      break;
    }
    songs_message += song["result"]["full_title"];
    songs_message += "\nСcылка: ";
    songs_message += song["result"]["url"];
    songs_message += "\n\n";
  }

  return songs_message;
}

void Cmds::new_post_event(const long& from_id, const long& id) {
  _vk_handler.init_conservations();
  for (long conservation : _vk_handler.conservations) {
    string attachment = to_string(from_id) + '_' + to_string(id);
    request(append_vkurl("messages.send"),
     {{"message",      "Таки новый пост в группе🌚" },
      {"random_id",    "0"                         },
      {"peer_id",      to_string(conservation)     },
      {"attachment",   "wall" + attachment         },
      {"access_token", access_token                },
      {"v",            api_version                 }});
  }
}

string google_urlencode(const string& str) {
  string res;
  for (char c : str) {
    if (c == ' ') {
      res += '+';
    } else {
      res += c;
    }
  }
  return res;
}

string Cmds::google_cmd() {
  if (not _reply.is_null()) {
    return "https://www.google.com/search?q=" + google_urlencode(_reply["text"].get<string>());
  }
  return "";
}
