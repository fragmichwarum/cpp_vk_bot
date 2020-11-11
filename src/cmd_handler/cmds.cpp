#include "cmd_handler.hpp"
#include "lib/crc32.hpp"

using namespace bot;
using namespace cURL;

using nlohmann::json;

using std::string;
using std::vector;
using std::to_string;

Vk_cmd_handler::Vk_cmd_handler(Cmds& handler) {
  _handler = &handler;
}

string Vk_cmd_handler::get_args(const string& message) {
  vector<string> splitted = split(message);
  splitted.erase(splitted.begin());
  return std::accumulate(splitted.begin(), splitted.end(), string{},
                         [](string& cmd, string& el) -> decltype (auto) { return cmd += el + ' '; });
}

string Vk_cmd_handler::empty_args() noexcept(true) {
  return "Задана пустая строка.";
}

void Vk_cmd_handler::message_send(const string& text, const long& peer_id) {
  request(append_vkurl("messages.send"),
          {{ "message",      text               },
           { "peer_id",      to_string(peer_id) },
           { "random_id",    "0"                },
           { "access_token", access_token       },
           { "v",            api_version        },
           { "disable_mentions", "1"          }});
}

string Vk_cmd_handler::attachment_type(const string& method) {
  if (method == "photos.search") {
    return "photo";
  }
  if (method == "video.search") {
    return "video";
  }
  if (method == "docs.search") {
    return "doc";
  }
  return "";
}

string Vk_cmd_handler::media_not_found(const string& type) {
  if (type == "photo") {
    return "Не найдено фотографий!";
  }
  if (type == "video") {
    return "Не найдено видеозаписей!";
  }
  if (type == "doc") {
    return "Не найдено такого документа!";
  }
  return "";
}

string Vk_cmd_handler::media_search(const string& method, const string& text, const long& peer_id) {
  json vkmedia =
    json::parse(request(append_vkurl(method),
      {{ "q",            get_args(text)  },
       { "access_token", user_token      },
       { "v",            api_version     },
       { "count",        "50"            }}));

  string docs;
  json items = vkmedia["response"]["items"];

  if (items.size() == 0) {
    message_send(media_not_found(attachment_type(method)), peer_id);
    return "";
  }

  for (uint8_t i = 0; i < items.size() && i < 10; i++) {
    long index = rand() % items.size();
    docs +=
      attachment_type(method) +
      to_string(items[index]["owner_id"].get<long>()) + '_' +
      to_string(items[index][      "id"].get<long>()) + ',';
  }

  request(append_vkurl("messages.send"),
                 {{ "attachment",   docs               },
                  { "peer_id",      to_string(peer_id) },
                  { "access_token", access_token       },
                  { "random_id",    "0"                },
                  { "v",            api_version      }});
  return "";
}

string Cmds::document_cmd(_Cmd_ref cmd) {
  if (cmd.message == "+доки") {
    return _vk_handler.empty_args();
  }
  _vk_handler.media_search("docs.search", cmd.message, cmd.peer_id);
  return "";
}

string Cmds::picture_cmd(_Cmd_ref cmd) {
  if (cmd.message == "+пикча") {
    return _vk_handler.empty_args();
  }
  _vk_handler.media_search("photos.search", cmd.message, cmd.peer_id);
  return "";
}

string Cmds::video_cmd(_Cmd_ref cmd) {
  if (cmd.message == "+видео") {
    return _vk_handler.empty_args();
  }
  _vk_handler.media_search("video.search", cmd.message, cmd.peer_id);
  return "";
}

string Cmds::weather_cmd(_Cmd_ref cmd) {
  if (cmd.message == "+погода") {
    return _vk_handler.empty_args();
  }

  json weather =
    json::parse(request("http://api.openweathermap.org/data/2.5/weather?",
     {{"lang",  "ru"               },
      {"units", "metric"           },
      {"APPID", "ef23e5397af13d705cfb244b33d04561"},
      {"q",      split(cmd.message)[1]}}));

  if (weather["weather"].is_null()) {
    return "Нет такого города.";
  }

  string description = weather["weather"][0]["description"];
  int temp           = weather["main"]["temp"];
  int feels_like     = weather["main"]["feels_like"];
  int humidity       = weather["main"]["humidity"];
  string city_name   = weather["name"];
  return
    "Сейчас в  " + city_name + " " + to_string(temp) +
    "°C, " + description + "\nОщущается как " +
    to_string(feels_like) + "°C\nВлажность: " +
    to_string(humidity) + "%";
}

json wiki_search(const string& wiki_url, const params& text) {
  return json::parse(request(wiki_url, text));
}

string Cmds::wiki_cmd(_Cmd_ref cmd) {
  if (cmd.message == "+вики") {
    return _vk_handler.empty_args();
  }
  string wiki_url = "https://ru.wikipedia.org/w/api.php?";

  try {
    string page;
    json parsed =
      wiki_search(wiki_url,
       {{"titles",      _vk_handler.get_args(cmd.message) },
        {"action",      "query"    },
        {"format",      "json"     },
        {"prop",        "extracts" },
        {"exintro",     ""         },
        {"explaintext", ""         },
        {"redirects",   "1"        }});
    for (const auto& i : parsed["query"]["pages"].get<json::object_t>()) {
      page = i.first;
      break;
    }

    if (page != "-1") {
      return parsed["query"]["pages"][page]["extract"];
    }

    parsed = wiki_search(wiki_url,
     {{"action", "query"},
      {"list", "search"},
      {"format","json"},
      {"srsearch", curl_easy_escape(NULL,
                     _vk_handler.get_args(cmd.message).c_str(),
                     _vk_handler.get_args(cmd.message).length())}});
    if (parsed["query"]["search"].size() == 0) {
      return "Такой статьи не найдено.";
    }
    return parsed["query"]["search"][0]["snippet"];

  } catch(nlohmann::json::parse_error&) {

  } catch (nlohmann::json::type_error&) {

  }
  return "";
}

static string laugh(size_t len = 10) {
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

string Cmds::laugh_cmd(_Cmd_ref cmd) {
  vector<string> _args = split(cmd.message);
  if (_args.size() == 1) {
    return laugh();
  }

  if (_args[1] != "-s" && _args.size() > 1) {
    return "Неверный параметр.";
  }

  if (_args[1] == "-s" && _args.size() == 2) {
    return "Введи количество символов.";
  }

  if (_args[1] != "-s" && _args.size() >= 3) {
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

string Cmds::reverse_cmd([[maybe_unused]] _Cmd_ref cmd) {
  setlocale(LC_CTYPE, "");
  if (cmd.message == "+реверс") {
    return _vk_handler.empty_args();
  }
  return reverse(_vk_handler.get_args(cmd.message).c_str());
}

string Cmds::currency_cmd([[maybe_unused]] _Cmd_ref cmd) {
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
  for (const string& currency : currency_list) {
    result += to_string(parsed["Valute"][currency]["Nominal"].get<long>());
    result += ' ';
    result += parsed["Valute"][currency]["Name"];
    result += " -> ";
    result += to_string(parsed["Valute"][currency]["Value"].get<double>()) + "₽\n";
  }
  return result;
}

string Cmds::help_cmd([[maybe_unused]] _Cmd_ref cmd) {
  uint8_t PAGE_SIZE = 15;
  uint8_t count_of_pages = vk_cmds.size() / PAGE_SIZE;

  if (cmd.message == "+помощь") {
    return
      "+помощь <номер страницы>\n"
      "Индексация, конечно, начинается с нуля :)\n"
      "Максимальное количество страниц: " +
      to_string(count_of_pages + 1);
  }

  vector<string> args = split(cmd.message);

  if (not std::all_of(args[1].begin(), args[1].end(), ::isdigit)) {
    return "Аргумет не является числом.";
  }

  unsigned long page_number = stol(args[1]);

  if (page_number > count_of_pages) {
    return "Такой страницы ещё нет.";
  }

  string help_info = "Список команд:\n";

  cmds_t::const_iterator iterator = vk_cmds.begin();
  std::advance(iterator, page_number * PAGE_SIZE);

  for (uint8_t runner = 0; iterator != vk_cmds.end() && runner++ < PAGE_SIZE; iterator++) {
    help_info += iterator->first + " - " + std::get<string>(iterator->second) + "\n";
  }

  help_info += "\n...\nСтраница " + to_string(page_number) + " из " + to_string(count_of_pages);

  return help_info;
}

string Cmds::about_cmd([[maybe_unused]] _Cmd_ref cmd) {
  return
    "C++ bot,\n"
    "сурсы лежат тут: https://github.com/oxfffffe/cpp_vk_bot\n"
    "бота создал: @jijijijijijijijijijijijji (он)\n"
    "версия VK API: " + api_version + '\n' +
    "собран: " + _vk_handler.build_time;
}

static inline string long_to_hex_str(unsigned long digit) noexcept(true) {
  static constexpr char const alphabet[0x10] = {
    '0', '1', '2', '3',
    '4', '5', '6', '7',
    '8', '9', 'a', 'b',
    'c', 'd', 'e', 'f'
  };

  string hex;
  do {
    hex.insert(hex.begin(), alphabet[ digit % 0x10 ] );
  } while ((digit /= 0x10) > 0);
  return hex;
}

string Cmds::crc32_cmd(_Cmd_ref cmd) {
  if (cmd.message == "+crc32") {
    return _vk_handler.empty_args();
  }
  return "0x" + long_to_hex_str(crc32gen(_vk_handler.get_args(cmd.message).c_str()));
}

static string os_exec(string const& cmd) {
  string result;
  std::array<char, 128> buffer;
  std::unique_ptr<FILE, decltype (&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

string Cmds::os_cmd(_Cmd_ref cmd) {
  return os_exec(_vk_handler.get_args(cmd.message));
}

string Cmds::repeat_cmd(_Cmd_ref cmd) {
  return _vk_handler.get_args(cmd.message);
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
  std::array<char, 128> line;

  while (fgets(line.data(), 128, file) != NULL) {
    if (strncmp(line.data(), param.c_str(), param.size()) == 0) {
      return lineparse(line.data());
    }
  }
  fclose(file);
  return "";
}

string Cmds::stat_cmd([[maybe_unused]] _Cmd_ref cmd) {
  return
    "Всего памяти: "      + procinfo("/proc/meminfo", "MemTotal:") + "KiB.\n"
    "Использовано ОЗУ: "  + procinfo("/proc/self/status", "VmRSS:") + "KiB.\n"
    "Потоков занято: "    + procinfo("/proc/self/status", "Threads:") + '\n' +
    "Аптайм: "            + os_exec("ps -eo lstart,etime,cmd | grep vk | awk '{print $6}' | head -1") +
    "Команд обработано: " + to_string(_vk_handler.msg_counter);
}

string Cmds::ping_cmd([[maybe_unused]] _Cmd_ref cmd) {
  string ping = "Pinging users.get\n";
  float total = 0;
  for (uint8_t iter = 1; iter < 11; iter++) {
    clock_t now = clock();
    request(append_vkurl("users.get"), {
              { "user_ids",     "0"          },
              { "access_token", access_token },
              { "v",            api_version  }
           });
    float delay = (float)(clock() - now) / CLOCKS_PER_SEC * 10000;
    total += delay;
    ping += to_string(iter) + ". " + to_string(delay) + " ms\n";
  }
  ping += "-------------\n";
  ping += "Avg: " + to_string(total / 10.0f);
  return ping;
}

string Cmds::online_cmd(_Cmd_ref cmd) {
  json parsed =
    json::parse(request(append_vkurl("messages.getConversationMembers"),
     {{ "fields",       "online"     },
      { "peer_id",      to_string(cmd.peer_id)},
      { "random_id",    "0"          },
      { "access_token", access_token },
      { "v",            api_version  }}));
  if (not parsed["error"].is_null() &&
          parsed["error"]["error_code"] == 917L)
  {
    return "Упс, кажется у бота нет админки.";
  }
  string people = "Список людей онлайн:\n";
  for (const auto& profile : parsed["response"]["profiles"]) {
    if (profile["online"] == 1) {
      people += "@id" + to_string(profile["id"].get<long>()) + "(";
      people += profile["first_name"].get<string>() + " " +
                profile["last_name"].get<string>() + ")\n";
    }
  }
  return people;
}

static inline string ret_id(const string& id) {
  /** [@id123456789|...] */
  /**     ^       ^      */
  /**     3       9      */
  return id.substr(3, 9);
}

string Cmds::kick_cmd(_Cmd_ref cmd) {
  if (cmd.message == "+кик" && _reply.is_null()) {
    return _vk_handler.empty_args();
  }

  params body;

  if (not _reply.is_null()) {
    body = {{"chat_id", to_string(cmd.peer_id - 2000000000)},
            {"user_id", to_string(_reply["from_id"].get<long>())}};
  } else {
    body = {{"chat_id", to_string(cmd.peer_id - 2000000000)},
            {"user_id", ret_id(_vk_handler.get_args(cmd.message))}};
  }

  append_vkparams(body);
  json response = json::parse(request(append_vkurl("messages.removeChatUser"), body));
  if (not response["error"].is_null() &&
          response["error"]["error_code"] == 100)
  {
    return "Что-то пошло не так.";
  }
  if (not response["error"].is_null() &&
          response["error"]["error_code"] == 15)
  {
    return "Не могу кикнуть этого юзера/группу.";
  }

  if (response["error"].is_null()) {
    return "Arbeit macht frei";
  }
  return "";
}

string Cmds::blacklist_cmd(_Cmd_ref cmd) {
  if (cmd.message == "+мут") {
    return _vk_handler.empty_args();
  }

  vector<string> args = split(cmd.message);

  if (args.size() != 2) {
    return "Что-то пошло не так, проверь правильность аргументов.";
  }

  _vk_handler.database.insert_role(
    std::stol(ret_id(args[1])),
    cmd.peer_id,
    "мут"
  );
  return "Готово.";
}

string Cmds::role_cmd(_Cmd_ref cmd) {
  if (cmd.message == "+мут") {
    return _vk_handler.empty_args();
  }

  vector<string> args = split(cmd.message);

  if (args.size() != 3) {
    return "Что-то пошло не так, проверь правильность аргументов.";
  }

  _vk_handler.database.insert_role(
    std::stol(ret_id(args[1])),
    cmd.peer_id,
    args[2]
  );
  return "Роль успешно установлена";
}

string Cmds::get_roles_cmd(_Cmd_ref cmd) {
  if (cmd.message == "+роли") {
    return "Что-то пошло не так, проверь правильность аргументов.";
  }

  vector<string> args = split(cmd.message);

  vector<long> roles = _vk_handler.database.get_by_role(cmd.peer_id, args[1]);

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

string Cmds::complete_cmd(_Cmd_ref cmd) {
  if (cmd.message == "+дополни") {
    return _vk_handler.empty_args();
  }
  string body = _vk_handler.get_args(cmd.message);
  json parsed =
    json::parse(requestdata("https://pelevin.gpt.dobro.ai/generate/",
      to_json({{"prompt", body}, {"length", "50"}})));

  return body + parsed["replies"][0].get<string>();
}

struct non_alpha {
  inline bool operator()(char c) {
    return not (isdigit(c) or isalpha(c));
  }
};

inline bool is_latin(const string& text) {
  return std::find_if(text.begin(), text.end(), non_alpha()) == text.end();
}

string github_get_user_repo(const string& user, const string& repo_name) {
  json repos =
    json::parse(request("https://api.github.com/users/" + user + "/repos", {}));
  if (repos.is_object()) {
    return "Упс, кажется такого юзера нет.";
  }
  string founded_repo = "Информация о репозитории ";
  bool found = false;
  for (json repo : repos) {
    if (repo["name"] == repo_name) {
      found = true;
      founded_repo += repo["name"];
      founded_repo += ":\n";
      founded_repo += "Описание: ";
      founded_repo += not repo["description"].is_null() ? repo["description"] : "отсутствует";
      founded_repo += "\nЗвёзд: ";
      founded_repo += to_string(repo["stargazers_count"].get<long>());
      founded_repo += "\nДоминирующий язык: ";
      founded_repo += not repo["language"].is_null() ? repo["language"] : "отсутствует";
      founded_repo += "\nСсылка: ";
      founded_repo += repo["html_url"];
      founded_repo += "\nЕсли хочешь склонить: ";
      founded_repo += repo["clone_url"];
      founded_repo += "\nФорк: ";
      founded_repo += repo["fork"].get<bool>() ? "да\n\n" : "нет\n\n";
      break;
    }
  }
  if (not found) {
    return "Упс, кажется у этого юзера нет такого репозитория.";
  }
  return founded_repo;
}

string github_get_user_repos(const string& user) {
  json repos =
    json::parse(request("https://api.github.com/users/" + user + "/repos", {}));
  if (repos.is_object()) {
    return "Упс, кажется такого юзера нет.";
  }
  string user_repos = "Информация о репозиториях:\n";
  for (uint8_t i = 0; i < repos.size(); i++) {
    if (i == 5) {
      break;
    }
    user_repos += repos[i]["name"];
    user_repos += ":\n";
    user_repos += "Описание: ";
    user_repos += not repos[i]["description"].is_null() ? repos[i]["description"] : "отсутствует";
    user_repos += "\nЗвёзд: ";
    user_repos += to_string(repos[i]["stargazers_count"].get<long>());
    user_repos += "\nДоминирующий язык: ";
    user_repos += not repos[i]["language"].is_null() ? repos[i]["language"] : "отсутствует";
    user_repos += "\nСсылка: ";
    user_repos += repos[i]["html_url"];
    user_repos += "\nЕсли хочешь склонить: ";
    user_repos += repos[i]["clone_url"];
    user_repos += "\nФорк: ";
    user_repos += repos[i]["fork"].get<bool>() ? "да\n\n" : "нет\n\n";
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

string Cmds::github_info_cmd(_Cmd_ref cmd) {
  if (cmd.message == "+гитхаб") {
    return _vk_handler.empty_args();
  }

  vector<string> _args = split(cmd.message);
  string option = _args[1];
  string user;

  if (_args.size() == 3 or _args.size() == 4) {
    user = _args[2];
  }

  if (not is_latin(user)) {
    return "Недопустимые символы.";
  }

  vector<string> options = { "-репо", "-подписчики", "-юзер", "-помощь" };

  if (not std::any_of(options.begin(), options.end(), [&](string& __option){ return __option == option; })) {
    return "Неверный параметр. Напиши \"+гитхаб -помощь\", чтобы узнать о "
           "параметрах.";
  }

  if (_args.size() == 4 && option == "-репо") {
    return github_get_user_repo(user, _args[3]);
  }

  if (_args.size() == 3 && option == "-репо") {
    return github_get_user_repos(user);
  }

  if (_args.size() == 3 && option == "-юзер") {
    return github_get_user_info(user);
  }

  if (_args.size() == 3 && option == "-подписчики") {
    return github_get_user_followers(user);
  }

  if (_args.size() == 2 && option == "-помощь") {
    return "+гитхаб -репо <никнейм> - получить информацию о репозиториях(до 5 шт.) пользователя,\n"
           "+гитхаб -репо <никнейм> <репозиторий> - получить информацию о конкретном пользователя,\n"
           "+гитхаб -подписчики <никнейм> - получить список людей, подписанных на <никнейм>,\n"
           "+гитхаб -юзер <никнейм> - получить информацию о аккаунте GitHub.";
  }
  return "";
}

string Cmds::genius_cmd(_Cmd_ref cmd) {
  if (cmd.message == "+трек") {
    return _vk_handler.empty_args();
  }
  string genius_token = "JSgH4gUYSn3S2C6Wd4BUhGuV1FWaKSET9DQVl-HBqlqeQ3isoW5bXSllR90VKvQF";
  json songs =
    json::parse(request("https://api.genius.com/search?",
     {{"q", _vk_handler.get_args(cmd.message) },
      {"access_token", genius_token           }}));

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

void Cmds::new_post_event(const json& event) {
  _vk_handler.conservations = _vk_handler.database.get_peer_ids();

  long from_id = event["from_id"].get<long>();
  long id = event["id"].get<long>();

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
  string encoded(str.size(), ' ');
  std::transform(str.begin(), str.end(), encoded.begin(), [](char _Char){ return _Char == ' ' ? '+' : _Char; });
  return encoded;
}

string Cmds::google_cmd([[maybe_unused]] _Cmd_ref cmd) {
  if (not _reply.is_null()) {
    return "https://www.google.com/search?q=" + google_urlencode(_reply["text"].get<string>());
  }
  return "";
}

string Cmds::turn_off_cmd([[maybe_unused]] _Cmd_ref cmd) {
  exit(0);
}
