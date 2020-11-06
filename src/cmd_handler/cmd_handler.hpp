#pragma once

#include <codecvt>

#include "../cmd_handler/lib/json.hpp"
#include "../sqlite/sqlite.hpp"
#include "../splitter/split.hpp"
#include "../logger/logger.hpp"
#include "../network/curl.hpp"
#include "../cmd_handler/lib/wchar.hpp"

namespace bot {

using namespace cURL;
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

class Cmds;

using command     = string;
using description = string;
using cmd_pointer = string (Cmds::*)(void);
using access      = uint8_t;
using cmds_t      = map<command, tuple<description, cmd_pointer, access>>;

extern cmds_t const cmds;

template <class T>
bool               any_of(vector<T>& vec, T id);
vector<string>     words_from_file(const string& filename);
string             utf8_to_lower(const string& text);

class Vk_cmd_handler {
private:
  Cmds*            _handler;
public:
  Vk_cmd_handler   (Cmds&);
 ~Vk_cmd_handler   ();
  string           empty_args          ();
  string           get_cmd_body        ();
  string           attachment_type     (const string& method);
  string           media_not_found     (const string& type);
  void             message_send        (const string& text);
  string           media_search        (const string& method);
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

class Cmds {
private:
  string           _message;
  long             _peer_id;
  long             _from_id;
  long             _msg_counter{0};
  vector<string>   _args;
  Vk_cmd_handler   _backend{*this};

public:
  friend class Vk_cmd_handler;

  void init_cmds(
    const string& message,
    const long&   peer_id,
    const long&   from_id
  );

  void stress_test(const string& peer_id);

  string crc32_cmd();

  string picture_cmd();

  string video_cmd();

  string document_cmd();

  string weather_cmd();

  string wiki_cmd();

  string laugh_cmd();

  string reverse_cmd();

  string currency_cmd();

  string help_cmd();

  string about_cmd();

  string online_cmd();

  string kick_cmd();

  string role_cmd();

  string get_roles_cmd();

  string blacklist_cmd();

  string forbid_word_cmd();

  string repeat_cmd();

  string complete_cmd();

  string github_info_cmd();

  string stat_cmd();

  string os_cmd();

  string ping_cmd();
};
} // namespace handler
