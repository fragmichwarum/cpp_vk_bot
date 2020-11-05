#include "cmd_handler.hpp"
#include "lib/crc32.hpp"

using namespace bot;

Cmd_backend::Cmd_backend(Cmd_handler& handler) {
  _handler = &handler;
  init_words_blacklist();
}

Cmd_backend::~Cmd_backend()
{ delete _handler, _handler = nullptr; }

string Cmd_backend::get_cmd_body() {
  bool isspace = false;
  string param;
  for (char c : _handler->_message) {
    if (isspace) {
      param += c;
    }
    if (c == ' ') {
      isspace = true;
    }
  }
  return param;
}

void Cmd_backend::message_send(const string& text) {
  string vkurl = append_vkurl("messages.send");
  params body;

  body["message"] = text;
  body["peer_id"] = to_string(_handler->_peer_id);
  body["disable_mentions"] = "1";
  append_vkparams(body);
  request(vkurl, body);
}

void Cmd_backend::media_search(const string& method) {
  json parsed =
      json::parse(request(append_vkurl(method),
        {{ "q",            get_cmd_body() },
         { "access_token", user_token      },
         { "count",        "100"           },
         { "v",            api_version     }}));
  long size = parsed["response"]["items"].size();
  if (size == 0) {
    media_not_found(attachment_type(method));
    return;
  }

  string attachments;
  long index;

  for (int i = 0; i < size; i++) {
    if (i == 10) {
      break;
    }
    index = rand() % size;
    string owner_id = to_string(parsed["response"]["items"][index]["owner_id"].get<long>());
    string id       = to_string(parsed["response"]["items"][index]["id"].get<long>());
    attachments    += attachment_type(method) + owner_id + '_' + id + ',';
  }
  params attachment_body = {{"attachment", attachments },{"peer_id", to_string(_handler->_peer_id)}};
  append_vkparams(attachment_body);
  string vkurl = append_vkurl("messages.send");
  request(vkurl, attachment_body);
}

string Cmd_backend::attachment_type(const string& method) {
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

void Cmd_backend::media_not_found(const string& type) {
  if (type == "photo") {
    message_send("Не найдено фотографий!");
  } else if (type == "video") {
    message_send("Не найдено видеозаписей!");
  } else if (type == "doc") {
    message_send("Не найдено такого документа!");
  }
}

void Cmd_backend::empty_args() {
  message_send("Задана пустая строка.");
}

void Cmd_handler::document_cmd() {
  if (_message == "+документ" or _message == "+доки") {
    _backend.empty_args();
  } else {
    _backend.media_search("docs.search");
  }
}

void Cmd_handler::picture_cmd() {
  if (_message == "+пикча" or _message == "+фото") {
    _backend.empty_args();
  } else {
    _backend.media_search("photos.search");
  }
}

void Cmd_handler::video_cmd() {
  if (_message == "+видео" or _message == "+видос") {
    _backend.empty_args();
  } else {
    _backend.media_search("video.search");
  }
}

void Cmd_handler::weather_cmd() {
  if (_message == "+погода") {
    _backend.empty_args();
  } else {
    string open_weather_url = "http://api.openweathermap.org/data/2.5/weather?";
    json parsed =
        json::parse(request(open_weather_url,
         {{ "lang", "ru"                                },
          { "units", "metric"                           },
          { "APPID", "ef23e5397af13d705cfb244b33d04561" },
          { "q",     _args[1]                           }}));
    if (not parsed["weather"].is_null()) {
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

      _backend.message_send(weather);
    } else {
      _backend.message_send("Нет такого города.");
    }
  }
}

json wiki_search(const string& wiki_url, const params& text) {
  return json::parse(request(wiki_url, text));
}

void Cmd_handler::wiki_cmd() {
  if (_message == "+вики") {
    _backend.empty_args();
  } else {
    string wiki_url = "https://ru.wikipedia.org/w/api.php?";
    string page;
    json parsed;

    try {
      parsed =
        wiki_search(wiki_url,
         {{"titles",      _backend.get_cmd_body() },
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
        _backend.message_send(parsed["query"]["pages"][page]["extract"]);
        return;
      }
    } catch(nlohmann::json::parse_error&) {

    } catch (nlohmann::json::type_error&) {

    }

    try {
      parsed = wiki_search(wiki_url,
       {{"action", "query"},
        {"list", "search"},
        {"format","json"},
        {"srsearch", curl_easy_escape(NULL, _backend.get_cmd_body().c_str(), _backend.get_cmd_body().length())}});
      if (parsed["query"]["search"].size() == 0) {
        _backend.message_send("Такой статьи не найдено.");
        return;
      }
      _backend.message_send(parsed["query"]["search"][0]["snippet"]);
    } catch(nlohmann::json::parse_error&) {

    } catch (nlohmann::json::type_error&) {

    }
  }
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

//смех -5
void Cmd_handler::laugh_cmd() {
  if (_args.size() == 1) {
    _backend.message_send(laugh());
    return;
  }

  if (_args[1] != "-s" and _args.size() > 1) {
    _backend.message_send("Неверный параметр.");
    return;
  }

  if (_args[1] == "-s" and _args.size() == 2) {
    _backend.message_send("Введи количество символов.");
    return;
  }

  if (_args[1] != "-s" and _args.size() >= 3) {
    return;
  }

  if (_args[2][0] == '-') {
    _backend.message_send("Отрицательное количество символов, серьёзно?");
    return;
  }

  if (not std::all_of(_args[2].begin(), _args[2].end(), ::isdigit)) {
    _backend.message_send("Аргумент не является целым числом.");
    return;
  }

  if (_args[2].size() >= 6) {
    _backend.message_send("Слишком большое число.");
   return;
  }

  short length = std::stoi(_args[2]);

  if (length > 500) {
    _backend.message_send("Слишком большое число.");
    return;
  }

  if (length <= 0) {
    _backend.message_send("Длина строки 0?");
    return;
  }

  _backend.message_send(laugh(length));
}

void Cmd_handler::reverse_cmd() {
  setlocale(LC_CTYPE, "");
  if (_message == "+реверс") {
    _backend.empty_args();
  } else {
    _backend.message_send(reverse(_backend.get_cmd_body().c_str()));
  }
}

void Cmd_handler::currency_cmd() {
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
  _backend.message_send(result);
}

void Cmd_handler::help_cmd() {
  string help_info = "Список команд:\n";
  for (auto cmd : cmds) {
    help_info += cmd.first + " - " + std::get<string>(cmd.second) + '\n';
  }
  _backend.message_send(help_info);
}

void Cmd_handler::about_cmd() {
  string about =
    "C++ bot,\n"
    "сурсы лежат тут: https://github.com/oxfffffe/cpp_vk_bot\n"
    "бота создал: @jijijijijijijijijijijijji (он)\n"
    "версия VK API: " + api_version + '\n' +
    "собран: " + _backend.build_time;
  _backend.message_send(about);
}

void Cmd_handler::crc32_cmd() {
  if (_message == "+crc32") {
    _backend.empty_args();
  } else {
    unsigned long crc32 = crc32gen(_backend.get_cmd_body().c_str());
    stringstream stream;
    stream << "0x" << std::hex << crc32;
    _backend.message_send(stream.str());
  }
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

void Cmd_handler::os_cmd() {
  string cmd = _backend.get_cmd_body();
  _backend.message_send(os_exec(cmd));
}

void Cmd_handler::repeat_cmd() {
  _backend.message_send( _backend.get_cmd_body());
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

void Cmd_handler::stat_cmd() {
  string stat =
    "Всего памяти: "      + procinfo("/proc/meminfo", "MemTotal:") + "KiB.\n"
    "Использовано ОЗУ: "  + procinfo("/proc/self/status", "VmRSS:") + "KiB.\n"
    "Потоков занято: "    + procinfo("/proc/self/status", "Threads:") + '\n' +
    "Аптайм: "            + os_exec("ps -eo lstart,etime,args | grep vk | awk '{print $6}' | head -1") +
    "Команд обработано: " + to_string(_msg_counter);
  _backend.message_send(stat);
}

void Cmd_handler::ping_cmd() {
  clock_t now = clock();
  request(append_vkurl("groups.getLongPollServer"), {
            { "group_id",     group_id     },
            { "access_token", access_token },
            { "v",            api_version  }
         });
  _backend.message_send("Сообщение обработано за: " +
                to_string((float)(clock() - now)/CLOCKS_PER_SEC * 10000) +
                " ms.");
}

void Cmd_handler::online_cmd() {
  uint16_t access_err = 917;
  string vkurl = append_vkurl("messages.getConversationMembers");
  params body  = {{"fields", "online"},{"peer_id", to_string(_peer_id)}};
  append_vkparams(body);

  json parsed = json::parse(request(vkurl, body));
  if (not parsed["error"].is_null() and
          parsed["error"]["error_code"] == access_err)
  {
    _backend.message_send("Упс, кажется у бота нет админки.");
    return;
  }
  string people = "Список людей онлайн:\n";
  for (auto profile : parsed["response"]["profiles"]) {
    if (profile["online"] == 1) {
      people += "@id" + to_string(profile["id"].get<long>()) + "(";
      people += profile["first_name"].get<string>() + " " + profile["last_name"].get<string>() + ")\n";
    }
  }
  _backend.message_send(people);
}

string Cmd_backend::ret_id(const string& id) {
  long id_len = 9;
  return id.substr(3, id_len);
}

void Cmd_handler::kick_cmd() {
  if (_message == "+кик") {
    _backend.empty_args();
    return;
  }
  params body = {{"chat_id", to_string(_peer_id - 2000000000)},
                 {"user_id", _backend.ret_id(_backend.get_cmd_body())}};
  append_vkparams(body);
  json response = json::parse(request(append_vkurl("messages.removeChatUser"), body));
  if (not response["error"].is_null() and
          response["error"]["error_code"] == 100)
  {
    _backend.message_send("Что-то пошло не так.");
  }
}

void Cmd_handler::role_cmd() {
  if (_message == "+роль") {
    _backend.empty_args();
    return;
  }
  if (_args.size() == 3) {
    long id = std::stol(_backend.ret_id(_args[1]));
    _backend.database.insert_role(id, _peer_id, _args[2]);
    _backend.message_send("Роль успешно установлена.");
  } else {
    _backend.message_send("Что-то пошло не так, проверь правильность аргументов.");
  }
}
void Cmd_handler::get_roles_cmd() {
  if (_args.size() < 1) {
    _backend.message_send("Что-то пошло не так, проверь правильность аргументов.");
  } else {
    vector<uint32_t> roles = _backend.database.get_by_role(_peer_id, _args[1]);
    if (roles.size() == 0) {
      _backend.message_send("В этом чате нет участников с данной ролью.");
      return;
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
    _backend.message_send(moderators);
  }
}

void Cmd_handler::blacklist_cmd() {
  if (_message == "+мут") {
    _backend.empty_args();
    return;
  }
  if (_args.size() == 2) {
    long id = std::stol(_backend.ret_id(_args[1]));
    _backend.database.insert_role(id, _peer_id, "мут");
    _backend.message_send("Готово.");
  } else {
    _backend.message_send("Что-то пошло не так, проверь правильность аргументов.");
  }
}

void Cmd_handler::complete_cmd() {
  if (_message == "+дополни") {
    _backend.empty_args();
  } else {
    json parsed =
        json::parse(requestdata(
          "https://pelevin.gpt.dobro.ai/generate/",to_json({{"prompt",_backend.get_cmd_body()},{"length","50"}})));
    _backend.message_send(_backend.get_cmd_body() + parsed["replies"][0].get<string>());
  }
}

void Cmd_handler::forbid_word_cmd() {
  if (_message == "+запрети") {
    _backend.empty_args();
  } else {
    ofstream _log (word_blacklist, std::ios::app);
    _log << _args[1] << "\n";
    _log.close();
    _backend.init_words_blacklist();

    _backend.message_send("Слово было запрещено.");
  }
}

struct non_alpha {
  bool operator()(char c) {
    return not (isdigit(c) or isalpha(c));
  }
};

bool is_latin(const string& text) {
  return std::find_if(text.begin(), text.end(), non_alpha()) == text.end();
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
    user_repos += "Ссылка: ";
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

void Cmd_handler::github_info_cmd() {
  if (_message == "+гитхаб") {
    _backend.empty_args();
    return;
  }

  string option = _args[1];
  string user;

  if (_args.size() == 3) {
    user = _args[2];
  }

  if (not is_latin(user)) {
    _backend.message_send(
      "Недопустимые символы."
    );
    return;
  }

  vector<string> options = {
    "-репо",
    "-подписчики",
    "-юзер",
    "-помощь"
  };

  if (not _any_of(options, option)) {
    _backend.message_send(
      "Неверный параметр."
    );
    return;
  }

  if (_args.size() == 3 and option == "-репо") {
    _backend.message_send(github_get_user_repos(user));
  }

  if (_args.size() == 3 and option == "-юзер") {
    _backend.message_send(github_get_user_info(user));
  }

  if (_args.size() == 3 and option == "-подписчики") {
    _backend.message_send(github_get_user_followers(user));
  }

  if (_args.size() == 2 and option == "-помощь") {
    _backend.message_send(
      "+гитхаб -репо <никнейм> - получить информацию о репозиториях(до 5 шт.) пользователя,\n"
      "+гитхаб -подписчики <никнейм> - получить список людей, подписанных на <никнейм>,\n"
      "+гитхаб -юзер <никнейм> - получить информацию о аккаунте GitHub."
    );
  }
}
