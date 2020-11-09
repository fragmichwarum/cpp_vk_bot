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

typedef struct {
  string message;
  long peer_id;
} cmd_args;

using _Cmd_ref = const cmd_args&;

class Cmds;

using command     = string;
using description = string;
using cmd_pointer = string (Cmds::*)(_Cmd_ref);
using access      = uint8_t;
using cmds_t      = map<command, tuple<description, cmd_pointer, access>>;

extern cmds_t const vk_cmds;

vector<string>     words_from_file(const string& filename);
string             utf8_to_lower  (const string& text);
bool               exists         (const json& object, const string& key);

class Vk_cmd_handler {
private:
  Cmds*            _handler;

public:
  long             _msg_counter{};
  Vk_cmd_handler   (Cmds&);
 ~Vk_cmd_handler   () {};
  string           empty_args          ();
  string           get_args            (const string& message);
  string           attachment_type     (const string& method);
  string           media_not_found     (const string& type);
  void             message_send        (const string& text, const long& peer_id);
  string           media_search        (const string& method, const string& text, const long& peer_id);
  string           ret_id              (const string& nickname);
  void             log                 (const string& message);
  void             init_roles          (const long& peer_id);
  void             init_words_blacklist();
  void             init_conservations  ();
  vector<long>     moderators;
  vector<long>     blacklist;
  vector<string>   words_blacklist;
  vector<long>     conservations;
  Logger           logger{logfile, errfile};
  Database         database;
  string           build_time = logger._gen_time();
  json             parsed;
};

class Cmds {
private:
  json             _reply;
  Vk_cmd_handler   _vk_handler{*this};

public:
  friend class Vk_cmd_handler;

  Cmds(){};

  Cmds(const json& update) {
    init_cmds(update);
  }

  void init_cmds(const json& update);

  void stress_test(const string& peer_id);

  void new_post_event(const json& event);

  string crc32_cmd(_Cmd_ref args);

  string picture_cmd(_Cmd_ref args);

  string video_cmd(_Cmd_ref args);

  string document_cmd(_Cmd_ref args);

  string weather_cmd(_Cmd_ref args);

  string wiki_cmd(_Cmd_ref args);

  string laugh_cmd(_Cmd_ref args);

  string reverse_cmd(_Cmd_ref args);

  string currency_cmd(_Cmd_ref args);

  string help_cmd(_Cmd_ref args);

  string about_cmd(_Cmd_ref args);

  string online_cmd(_Cmd_ref args);

  string kick_cmd(_Cmd_ref args);

  string role_cmd(_Cmd_ref args);

  string get_roles_cmd(_Cmd_ref args);

  string blacklist_cmd(_Cmd_ref args);

  string forbid_word_cmd(_Cmd_ref args);

  string repeat_cmd(_Cmd_ref args);

  string complete_cmd(_Cmd_ref args);

  string github_info_cmd(_Cmd_ref args);

  string genius_cmd(_Cmd_ref args);

  string google_cmd(_Cmd_ref args);

  string turn_off_cmd(_Cmd_ref args);

  string stat_cmd(_Cmd_ref args);

  string os_cmd(_Cmd_ref args);

  string ping_cmd(_Cmd_ref args);
};
} // namespace handler
