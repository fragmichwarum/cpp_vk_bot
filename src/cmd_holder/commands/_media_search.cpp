#include "../cmd_holder.hpp"

namespace {
  using nlohmann::json;
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

void cmd_holder::_media_search(const vector<string>& tokenized_message, const string& method)
{
  string query = _message.substr(tokenized_message[0].size() + 1, _message.size() - 1);
  params search_body;
  search_body["q"]             = query;
  search_body["access_token"]  = user_token;
  search_body["count"]         = "100";
  search_body["v"]             = api_version;

  json parsed = http_processing(append_vkurl(method), search_body);
  long size = parsed["response"]["items"].size();
  long index;

  if (size == 0) {
    _media_not_found(_attachment_type(method));
    return;
  } else if (size == 1) {
    index = 0;
  }
  index           = rand() % size - 1;
  string owner_id = to_string(parsed["response"]["items"][index]["owner_id"].get<long>());
  string id       = to_string(parsed["response"]["items"][index][      "id"].get<long>());
  params attachment_body;
  attachment_body["attachment"] = _attachment_type(method) + owner_id + "_" + id;
  attachment_body["peer_id"] = to_string(_peer_id);
  append_vkparams(attachment_body);
  Curl::send_request(append_vkurl("messages.send"), attachment_body);
}
