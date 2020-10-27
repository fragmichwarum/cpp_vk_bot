#pragma once

#include "../cmd_handler/lib/json.hpp"
#include "../network/curl.hpp"
#include "../sqlite/sqlite.hpp"
#include "../splitter/split.hpp"
#include "../logger/logger.hpp"

namespace handler {
using namespace cURL;
using std::to_string;
using std::string;
using std::vector;
using std::array;
using std::unique_ptr;
using std::runtime_error;
using std::stringstream;
using nlohmann::json;
constexpr bool     USE_NICKNAME = true;
constexpr bool NOT_USE_NICKNAME = false;

class Cmd_handler {
private:
  Logger         _logger{logfile, errfile};
  Database       _database;
  string         _message;
  long           _peer_id;
  long           _from_id;
  string         _nickname;
  vector<string> _splitted_message;
  void           _empty_query    ();
  string         _get_cmd_body   ();
  string         _attachment_type(const string& method);
  void           _media_not_found(const string& type);
  void           _message_send   (const string& text, bool use_nickname);
  void           _media_search   (const string& method);
  void           _add_nickname   ();
  void           _remove_nickname();

public:

  void init_cmds(
    const string& message,
    const long&   peer_id,
    const long&   from_id
  );

  void crc32_cmd();

  void picture_cmd();

  void video_cmd();

  void document_cmd();

  void weather_cmd();

  void wiki_cmd();

  void translate_cmd();

  void help_cmd();

  void nickname_cmd();

  void repeat_cmd();

  void stat_cmd();

  void os_cmd();

  void ping_cmd();
};
} // namespace handler
