#include "../cmd_holder.hpp"


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
    _message_send("Не найдено фотографий!", NOT_USE_NICKNAME);
  }
  else if (type == "video") {
    _message_send("Не найдено видеозаписей!", NOT_USE_NICKNAME);
  }
  else if (type == "doc") {
    _message_send("Не найдено такого документа!", NOT_USE_NICKNAME);
  }
}

void cmd_holder::_empty_query() {
  _message_send("Задана пустая строка.", NOT_USE_NICKNAME);
}

void cmd_holder::_media_search(const vector<string>& tokenized_message, const string& method)
{
  string media_title =
    _message.substr(
      tokenized_message[0].size() + 1,
      _message.size() - 1);

  map<string, string> query;
  query["q"]            = media_title;
  query["access_token"] = user_token;
  query["count"]        = "100";
  query["v"]            = api_version;

  auto parsed =
    nlohmann::json::parse(
      Curl::request(
        Curl::generate_query(
          api_url +
          method +
          "?",
          query)));

  long size  = parsed["response"]["items"].size();
  long index = rand() % size - 1;

  if (size == 0) {
    _media_not_found(_attachment_type(method));
  }
  else if (size == 1) {
    index = 0;
  }
  string owner_id = to_string((long)parsed["response"]["items"][index]["owner_id"]);
  string id       = to_string((long)parsed["response"]["items"][index]["id"]);

  map<string, string> attachment_query;
  attachment_query["attachment"] = _attachment_type(method) + owner_id + "_" + id;
  attachment_query["peer_id"]    = to_string(_peer_id);
  if (_nickname != "") {
    attachment_query["message"] = _nickname + ",";
  }

  Curl::request(
    Curl::generate_vk_query(
      "messages.send",
      attachment_query));
}
