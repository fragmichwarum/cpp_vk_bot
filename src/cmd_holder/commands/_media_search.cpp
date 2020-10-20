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
  map<string, string> params = {
    { "q",            query       },
    { "access_token", user_token  },
    { "count",        "100"       },
    { "v",            api_version },
  };
  auto parsed = json::parse(Curl::send_request(append_vkurl(method), params));
  long size = parsed["response"]["items"].size();
  long index;

  if (size == 0) {
    _media_not_found(_attachment_type(method));
    return;
  } else if (size == 1) {
    index = 0;
  }
  index           = rand() % size - 1;
  string owner_id = to_string((long)parsed["response"]["items"][index]["owner_id"]);
  string id       = to_string((long)parsed["response"]["items"][index][      "id"]);
  map<string, string> attachment_params = {
    { "attachment", _attachment_type(method) + owner_id + "_" + id },
    { "peer_id",    to_string(_peer_id)                            }
  };
  append_vkparams(attachment_params);
  Curl::send_request(append_vkurl("messages.send"), attachment_params);
}
