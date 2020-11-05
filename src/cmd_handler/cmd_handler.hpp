#pragma once

#include <codecvt>
#include <thread>

#include "../cmd_handler/lib/json.hpp"
#include "../sqlite/sqlite.hpp"
#include "../splitter/split.hpp"
#include "../logger/logger.hpp"
#include "../network/curl.hpp"
#include "../cmd_handler/lib/wchar.hpp"

namespace bot {

using namespace cURL;
using std::thread;
using std::to_string;
using std::string;
using std::wstring;
using std::vector;
using std::array;
using std::tuple;
using std::unique_ptr;
using std::runtime_error;
using std::stringstream;
using std::invalid_argument;
using std::ofstream;
using nlohmann::json;

class Cmd_handler;

using command     = string;
using description = string;
using cmd_pointer = void (Cmd_handler::*)(void);
using access      = uint8_t;
using cmds_t      = map<command, tuple<description, cmd_pointer, access>>;

extern cmds_t const cmds;

template <class T>
bool               _any_of(vector<T>& vec, T id);
vector<string>     _words_from_file(const string& filename);
string             _utf8_to_lower(const string& text);

class Cmd_backend {
private:
  Cmd_handler*     _handler;
public:
  Cmd_backend      (Cmd_handler&);
 ~Cmd_backend      ();
  void             empty_args          ();
  string           get_cmd_body        ();
  string           attachment_type     (const string& method);
  void             media_not_found     (const string& type);
  void             message_send        (const string& text);
  void             media_search        (const string& method);
  string           ret_id              (const string& nickname);
  void             init_roles          ();
  void             init_words_blacklist();
  vector<uint32_t> moderators;
  vector<uint32_t> blacklist;
  vector<string>   words_blacklist;
  Logger           logger{logfile, errfile};
  Database         database;
  string           build_time = logger._gen_time();
};

class Cmd_handler {
private:
  string           _message;
  long             _peer_id;
  long             _from_id;
  long             _msg_counter{0};
  vector<string>   _args;
  Cmd_backend      _backend{*this};

public:
  friend class Cmd_backend;

  thread init_thread(
    const string& message,
    const long&   peer_id,
    const long&   from_id);

  void init_cmds(
    const string& message,
    const long&   peer_id,
    const long&   from_id
  );

  void stress_test(const string& peer_id);

  void crc32_cmd();

  void picture_cmd();

  void video_cmd();

  void document_cmd();

  void weather_cmd();

  void wiki_cmd();

  void laugh_cmd();

  void reverse_cmd();

  void currency_cmd();

  void help_cmd();

  void about_cmd();

  void online_cmd();

  void kick_cmd();

  void role_cmd();

  void get_roles_cmd();

  void blacklist_cmd();

  void forbid_word_cmd();

  void repeat_cmd();

  void complete_cmd();

  void github_info_cmd();

  void stat_cmd();

  void os_cmd();

  void ping_cmd();
};
} // namespace handler
