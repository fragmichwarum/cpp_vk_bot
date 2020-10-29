#pragma once

#include "../cmd_handler/lib/json.hpp"
#include "../sqlite/sqlite.hpp"
#include "../splitter/split.hpp"
#include "../logger/logger.hpp"
#include "../network/curl.hpp"

namespace bot {

using namespace cURL;
using std::to_string;
using std::string;
using std::vector;
using std::array;
using std::tuple;
using std::unique_ptr;
using std::runtime_error;
using std::stringstream;
using nlohmann::json;
constexpr bool USE_NICKNAME     = true;
constexpr bool NOT_USE_NICKNAME = false;
constexpr bool ADMIN            = true;
constexpr bool USER             = false;

class Cmd_handler;

using command     = string;
using description = string;
using cmd_pointer = void (Cmd_handler::*)(void);
using access      = bool;
using cmds_t      = unordered_map<command, tuple<description, cmd_pointer, access>>;

extern cmds_t const cmds;

class Cmd_backend {
private:
  Cmd_handler*   _handler;
public:
  Cmd_backend    (Cmd_handler&);
 ~Cmd_backend    ();
  void           _empty_query    ();
  void           _add_nickname   ();
  void           _remove_nickname();
  string         _get_cmd_body   ();
  string         _attachment_type(const string& method);
  void           _media_not_found(const string& type);
  void           _message_send   (const string& text, bool use_nickname);
  void           _media_search   (const string& method);
};

class Cmd_handler {
private:
  Logger         _logger{logfile, errfile};
  Database       _database;
  string         _message;
  long           _peer_id;
  long           _from_id;
  string         _nickname;
  long           _msg_counter{0};
  vector<string> _splitted_message;
  Cmd_backend    _backend{*this};

public:
  friend class Cmd_backend;

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

//  void translate_cmd();

  void help_cmd();

  void nickname_cmd();

  void repeat_cmd();

  void stat_cmd();

  void os_cmd();

  void ping_cmd();
};
} // namespace handler
