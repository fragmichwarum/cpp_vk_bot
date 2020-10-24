#include "cmd_holder.hpp"
#include "lib/crc32.hpp"
#include "lib/sha256.hpp"

using namespace holder;

namespace {
string os_exec(const string& cmd) {
  array<char, 128> buffer;
  string result;
  unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
  if (not pipe) {
    throw runtime_error("popen() failure");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

string lineparse(const string& line) {
  string result;
  for (char c : line) {
    if (isdigit(c)) {
      result += c;
    }
  }
  return result;
}

string memusage() {
  FILE* file = fopen("/proc/self/status", "r");
  string result;
  array<char, 128> line;

  while (fgets(line.data(), 128, file) != NULL) {
    if (strncmp(line.data(), "VmRSS:", 6) == 0) {
      return lineparse(line.data());
    }
  }
  fclose(file);
  return "";
}
} //namespace

string cmd_holder::_get_cmd_body() {
  bool isspace = false;
  string param;
  for (char c : _message) {
    if (isspace) {
      param += c;
    }
    if (c == ' ') {
      isspace = true;
    }
  }
  return param;
}

void cmd_holder::_message_send(const string& text, bool use_nickname) {
  bool   is_nickname = _nickname != "" and use_nickname;
  string peer_id     = to_string(_peer_id);
  string vkurl       = append_vkurl("messages.send");
  params body;

  if (is_nickname) {
    body["message"] =_nickname + ", " + text;
  } else {
    body["message"] = text;
  }
  body["peer_id"] = peer_id;
  append_vkparams(body);
  request(vkurl, body);
}

string cmd_holder::_attachment_type(const string& method) {
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

void cmd_holder::_media_not_found(const string& type) {
  if (type == "photo") {
    _message_send("Не найдено фотографий!",       NOT_USE_NICKNAME);
  } else if (type == "video") {
    _message_send("Не найдено видеозаписей!",     NOT_USE_NICKNAME);
  } else if (type == "doc") {
    _message_send("Не найдено такого документа!", NOT_USE_NICKNAME);
  }
}

void cmd_holder::_empty_query() {
  _message_send("Задана пустая строка.", NOT_USE_NICKNAME);
}

void cmd_holder::document_cmd() {
  if (_message == "+документ" or
      _message == "+доки")
  {
    _empty_query();
  } else {
    _media_search(_splitted_message, "docs.search");
  }
}

void cmd_holder::picture_cmd() {
  if (_message == "+пикча" or
      _message == "+фото")
  {
    _empty_query();
  } else {
    _media_search(_splitted_message, "photos.search");
  }
}

void cmd_holder::video_cmd() {
  if (_message == "+видео" or
      _message == "+видос")
  {
    _empty_query();
  } else {
    _media_search(_splitted_message, "video.search");
  }
}

void cmd_holder::_media_search(const vector<string>& tokenized_message, const string& method)
{
  string query = _message.substr(tokenized_message[0].size() + 1, _message.size() - 1);
  params search_body;
  search_body["q"]             = query;
  search_body["access_token"]  = user_token;
  search_body["count"]         = "100";
  search_body["v"]             = api_version;

  long index;
  json parsed  = json::parse(request(append_vkurl(method), search_body));
  long size    = parsed["response"]["items"].size();
  string vkurl = append_vkurl("messages.send");
  string attachments;
  params attachment_body;

  if (size == 0) {
    _media_not_found(_attachment_type(method));
    return;
  }
  for (int i = 0; i < size; i++) {
    if (i == 10) {
      break;
    }
    index           = rand() % size;
    string owner_id = to_string(parsed["response"]["items"][index]["owner_id"].get<long>());
    string id       = to_string(parsed["response"]["items"][index][      "id"].get<long>());
    attachments     += _attachment_type(method) + owner_id + '_' + id + ',';
  }
  attachment_body["attachment"] = attachments;
  attachment_body["peer_id"] = to_string(_peer_id);
  append_vkparams(attachment_body);
  request(vkurl, attachment_body);
}

void cmd_holder::weather_cmd() {
  if (_message == "+погода") {
    _empty_query();
  } else {
    params body;
    body["lang"]  = "ru";
    body["units"] = "metric";
    body["APPID"] = "ef23e5397af13d705cfb244b33d04561";
    body["q"]     = _splitted_message[1];
    json parsed   = json::parse(request("http://api.openweathermap.org/data/2.5/weather?", body));

    if (not parsed["weather"].is_null()) {
      string description = parsed["weather"][0]["description"];
      int temp           = parsed["main"]["temp"];
      int feels_like     = parsed["main"]["feels_like"];
      int humidity       = parsed["main"]["humidity"];
      string city_name   = parsed["name"];

      string weather = "Сейчас в  " + city_name + " " +
           to_string(temp) + "°C, " + description + "\nОщущается как " +
           to_string(feels_like)    + "°C\nВлажность: " +
           to_string(humidity)      + "%";

      _message_send(weather, NOT_USE_NICKNAME);
    } else {
      _message_send("Нет такого города.", NOT_USE_NICKNAME);
    }
  }
}

void cmd_holder::help_cmd() {
  string help_info =
    "Список команд:\n"
    "⚠+crc32 {...} - сгенерить CRC-32 хеш-сумму строки\n"
    "⚠+sha256 {...} - сгенерить SHA-256 хеш-сумму строки\n"
    "⚠+пикча, +фото {...} - найти картинку среди просторов ВК\n"
    "⚠+видео, +видос {...} - поиск видеозаписей\n"
    "⚠+документ, +доки {...} - поиск документов\n"
    "⚠+погода {...} - показать погоду\n"
    "⚠+никнейм {...} - установить никнейм\n"
    "⚠+никнейм удалить - удалить никнейм\n";
  _message_send(help_info, NOT_USE_NICKNAME);
}

void cmd_holder::crc32_cmd() {
  if (_message == "+crc32") {
    _empty_query();
  } else {
    unsigned long crc32 = crc32gen(_get_cmd_body().c_str());
    stringstream stream;
    stream << "0x" << std::hex << crc32;
    _message_send(stream.str(), USE_NICKNAME);
  }
}

void cmd_holder::sha256_cmd() {
  if (_message == "+sha256") {
    _empty_query();
  } else {
    SHA256 sha(_get_cmd_body().c_str());
    _message_send(sha.hash(), USE_NICKNAME);
  }
}

void cmd_holder::add_nickname_cmd() {
  _database.insert_nickname(_from_id, _splitted_message[1]);
  _message_send("Никнейм успешно установлен\n", NOT_USE_NICKNAME);
}

void cmd_holder::remove_nickname_cmd() {
  _database.insert_nickname(_from_id, "");
  if (_nickname != "") {
    _message_send("Никнейм успешно удалён", NOT_USE_NICKNAME);
  } else {
    _message_send("Никнейм не установлен", NOT_USE_NICKNAME);
  }
}

void cmd_holder::nickname_cmd() {
  if (_message == "+никнейм") {
    _empty_query();
  } else if (_splitted_message[1] == "удалить") {
    remove_nickname_cmd();
  } else {
    add_nickname_cmd();
  }
}

void cmd_holder::os_cmd() {
  string cmd = _get_cmd_body();
  _message_send(os_exec(cmd), NOT_USE_NICKNAME);
}

void cmd_holder::repeat_cmd() {
  if (_message != "+!") {
  string to_repeat = _get_cmd_body();
  _message_send(to_repeat, NOT_USE_NICKNAME);
  } else {
    _empty_query();
  }
}

void cmd_holder::stat_cmd() {
  _message_send("Использовано ОЗУ: " + memusage() + " KiB.\n" +
                "Аптайм: " + os_exec("ps -eo lstart,etime,args | grep vk | awk '{print $6}' | head -1"), NOT_USE_NICKNAME);
}

void cmd_holder::ping_cmd() {
  clock_t now = clock();
  request(append_vkurl("groups.getLongPollServer"), {
            { "group_id",     group_id     },
            { "access_token", access_token },
            { "v",            api_version  }
         });
  _message_send("Сообщение обработано за: " +
                to_string((float)(clock() - now)/CLOCKS_PER_SEC * 10000) +
                " ms.", NOT_USE_NICKNAME);
}
