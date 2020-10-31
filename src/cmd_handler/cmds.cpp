#include "cmd_handler.hpp"
#include "lib/crc32.hpp"

using namespace bot;

Cmd_backend::Cmd_backend(Cmd_handler& handler)
{ _handler = &handler; }

Cmd_backend::~Cmd_backend()
{ delete _handler; }

string Cmd_backend::_get_cmd_body() {
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

void Cmd_backend::_message_send(const string& text, bool use_nickname) {
  bool is_nickname = _handler->_nickname != "" and use_nickname;
  string vkurl = append_vkurl("messages.send");
  params body;

  if (is_nickname) {
    body["message"] = _handler->_nickname + ", " + text;
  } else {
    body["message"] = text;
  }
  body["peer_id"] = to_string(_handler->_peer_id);
  body["disable_mentions"] = "1";
  append_vkparams(body);
  request(vkurl, body);
}

void Cmd_backend::_media_search(const string& method) {
  json parsed =
      json::parse(request(append_vkurl(method),
        {{ "q",            _get_cmd_body() },
         { "access_token", user_token      },
         { "count",        "100"           },
         { "v",            api_version     }}));
  long size = parsed["response"]["items"].size();
  if (size == 0) {
    _media_not_found(_attachment_type(method));
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
    attachments    += _attachment_type(method) + owner_id + '_' + id + ',';
  }
  params attachment_body = {{"attachment", attachments },{"peer_id", to_string(_handler->_peer_id)}};
  append_vkparams(attachment_body);
  string vkurl = append_vkurl("messages.send");
  request(vkurl, attachment_body);
}

void Cmd_backend::_add_nickname() {
  _database.insert(_handler->_from_id,
                                      _handler->_splitted_message[1]);
  _message_send("Никнейм успешно установлен\n", NOT_USE_NICKNAME);
}

void Cmd_backend::_remove_nickname() {
  _database.insert(_handler->_from_id, "");
  if (_handler->_nickname != "") {
    _message_send("Никнейм успешно удалён", NOT_USE_NICKNAME);
  } else {
    _message_send("Никнейм не установлен", NOT_USE_NICKNAME);
  }
}

string Cmd_backend::_attachment_type(const string& method) {
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

void Cmd_backend::_media_not_found(const string& type) {
  if (type == "photo") {
    _message_send("Не найдено фотографий!", NOT_USE_NICKNAME);
  } else if (type == "video") {
    _message_send("Не найдено видеозаписей!", NOT_USE_NICKNAME);
  } else if (type == "doc") {
    _message_send("Не найдено такого документа!", NOT_USE_NICKNAME);
  }
}

void Cmd_backend::_empty_query() {
  _message_send("Задана пустая строка.", NOT_USE_NICKNAME);
}

void Cmd_handler::document_cmd() {
  if (_message == "+документ" or _message == "+доки") {
    _backend._empty_query();
  } else {
    _backend._media_search("docs.search");
  }
}

void Cmd_handler::picture_cmd() {
  if (_message == "+пикча" or _message == "+фото") {
    _backend._empty_query();
  } else {
    _backend._media_search("photos.search");
  }
}

void Cmd_handler::video_cmd() {
  if (_message == "+видео" or _message == "+видос") {
    _backend._empty_query();
  } else {
    _backend._media_search("video.search");
  }
}

void Cmd_handler::weather_cmd() {
  if (_message == "+погода") {
    _backend._empty_query();
  } else {
    string open_weather_url = "http://api.openweathermap.org/data/2.5/weather?";
    json parsed =
        json::parse(request(open_weather_url,
         {{ "lang", "ru"                                },
          { "units", "metric"                           },
          { "APPID", "ef23e5397af13d705cfb244b33d04561" },
          { "q",     _splitted_message[1]               }}));
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

      _backend._message_send(weather, NOT_USE_NICKNAME);
    } else {
      _backend._message_send("Нет такого города.", NOT_USE_NICKNAME);
    }
  }
}

/*
 * косяк: обрабатывает только запросы на английском
 */
void Cmd_handler::wiki_cmd() {
  if (_message == "+вики") {
    _backend._empty_query();
  } else {
    string wiki_url = "https://ru.wikipedia.org/w/api.php?";
    string page;
    json parsed;
    try {
      parsed =
        json::parse(request(wiki_url,
         {{"titles",      _backend._get_cmd_body() },
          {"action",      "query"    },
          {"format",      "json"     },
          {"prop",        "extracts" },
          {"exintro",     ""         },
          {"explaintext", ""         },
          {"redirects",   "1"        }}));
      for (auto i : parsed["query"]["pages"].get<json::object_t>()) {
        page = i.first;
        break;
      }
    } catch (json::parse_error& parse_error) {
      _backend._logger.write_err(__LINE__, __FILE__, __FUNCTION__, parse_error.what());
      return;
    } catch (nlohmann::detail::type_error& type_error) {
      _backend._logger.write_err(__LINE__, __FILE__, __FUNCTION__, type_error.what());
      return;
    }
    if (page != "-1") {
      _backend._message_send(parsed["query"]["pages"][page]["extract"], USE_NICKNAME);
    } else {
      _backend._message_send("Такой статьи не найдено.", NOT_USE_NICKNAME);
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

void Cmd_handler::laugh_cmd() {
  if (_splitted_message.size() == 1) {
    _backend._message_send(laugh(), USE_NICKNAME);
  } else {
    if (_splitted_message[1] == "-s" && _splitted_message.size() >= 3) {
      try {
        _backend._message_send(laugh(std::stoi(_splitted_message[2])), USE_NICKNAME);
      } catch(invalid_argument&) {
         _backend._message_send("Неверный параметр. Используй -s <число>.", USE_NICKNAME);
      }
    }
  }
}

void Cmd_handler::reverse_cmd() {
  setlocale(LC_CTYPE, "");
  if (_message == "+реверс") {
    _backend._empty_query();
  } else {
    _backend._message_send(reverse(_backend._get_cmd_body().c_str()), USE_NICKNAME);
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
  _backend._message_send(result, NOT_USE_NICKNAME);
}

void Cmd_handler::help_cmd() {
  string help_info = "Список команд:\n";
  for (auto cmd : cmds) {
    help_info += cmd.first + " - " + std::get<string>(cmd.second) + '\n';
  }
  _backend._message_send(help_info, NOT_USE_NICKNAME);
}

void Cmd_handler::about_cmd() {
  string about =
    "C++ bot,\n"
    "сурсы лежат тут: https://github.com/oxfffffe/cpp_vk_bot\n"
    "бота создал: @jijijijijijijijijijijijji (он)\n"
    "версия VK API: " + api_version + '\n' +
    "собран: " + _backend._build_time;
  _backend._message_send(about, NOT_USE_NICKNAME);
}

void Cmd_handler::crc32_cmd() {
  if (_message == "+crc32") {
    _backend._empty_query();
  } else {
    unsigned long crc32 = crc32gen(_backend._get_cmd_body().c_str());
    stringstream stream;
    stream << "0x" << std::hex << crc32;
    _backend._message_send(stream.str(), USE_NICKNAME);
  }
}

void Cmd_handler::nickname_cmd() {
  if (_message == "+никнейм") {
    _backend._empty_query();
  } else if (_splitted_message[1] == "удалить") {
    _backend._remove_nickname();
  } else {
    _backend._add_nickname();
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
  string cmd = _backend._get_cmd_body();
  _backend._message_send(os_exec(cmd), NOT_USE_NICKNAME);
}

void Cmd_handler::repeat_cmd() {
  _backend._message_send( _backend._get_cmd_body(), NOT_USE_NICKNAME);
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
  _backend._message_send(stat, NOT_USE_NICKNAME);
}

void Cmd_handler::ping_cmd() {
  clock_t now = clock();
  request(append_vkurl("groups.getLongPollServer"), {
            { "group_id",     group_id     },
            { "access_token", access_token },
            { "v",            api_version  }
         });
  _backend._message_send("Сообщение обработано за: " +
                to_string((float)(clock() - now)/CLOCKS_PER_SEC * 10000) +
                " ms.", NOT_USE_NICKNAME);
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
    _backend._message_send(
      "Упс, кажется у бота нет админки.",
      NOT_USE_NICKNAME
    );
    return;
  }
  string people;
  for (auto profile : parsed["response"]["profiles"]) {
    if (profile["online"] == 1) {
      people += "@id" + to_string(profile["id"].get<long>()) + "(";
      people += profile["first_name"].get<string>() + " " + profile["last_name"].get<string>() + ")\n";
    }
  }
  _backend._message_send(people, NOT_USE_NICKNAME);
}

string Cmd_backend::_ret_id(const string& nickname) {
  long id_len = 9;
  return nickname.substr(3, id_len);
}

void Cmd_handler::kick_cmd() {
  if (_message == "+кик") {
    _backend._empty_query();
    return;
  }
  params body = {{"chat_id", to_string(_peer_id - 2000000000)},{"user_id", _backend._ret_id(_backend._get_cmd_body())}};
  append_vkparams(body);
  json response = json::parse(request(append_vkurl("messages.removeChatUser"), body));
  if (not response["error"].is_null() and
          response["error"]["error_code"] == 100)
  {
    _backend._message_send("Что-то пошло не так.", NOT_USE_NICKNAME);
  }
}
