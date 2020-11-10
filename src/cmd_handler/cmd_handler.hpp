#pragma once

#include <codecvt>

#include "../cmd_handler/lib/json.hpp"
#include "../sqlite/sqlite.hpp"
#include "../splitter/split.hpp"
#include "../logger/logger.hpp"
#include "../network/curl.hpp"
#include "../cmd_handler/lib/wchar.hpp"

namespace bot {

typedef struct {
  std::string message;
  long peer_id;
} cmd_args;

using _Cmd_ref = const cmd_args&;

class Cmds;

using command     = std::string;
using description = std::string;
using cmd_pointer = std::string (Cmds::*)(_Cmd_ref);
using access      = uint8_t;
using cmds_t      = std::map<command, std::tuple<description, cmd_pointer, access>>;

extern cmds_t const vk_cmds;

std::vector<std::string> words_from_file(const std::string& filename);
std::string              utf8_to_lower  (const std::string& text);
bool                     exists         (const nlohmann::json& object, const std::string& key);

class Vk_cmd_handler {
private:
  Cmds*            _handler;

public:
  explicit
  Vk_cmd_handler    (Cmds&);
 ~Vk_cmd_handler    () {};
  std::string       empty_args          () noexcept(true);
  std::string       get_args            (const std::string& message);
  std::string       attachment_type     (const std::string& method);
  std::string       media_not_found     (const std::string& type);
  std::string       media_search        (const std::string& method, const std::string& text, const long& peer_id);
  void              message_send        (const std::string& text, const long& peer_id);
  void              log                 (const std::string& message);
  void              init_roles          (const long& peer_id);
  void              init_words_blacklist();
  void              init_conservations  ();
  std::vector<long> moderators;
  std::vector<long> blacklist;
  std::vector<std::string> words_blacklist;
  std::vector<long> conservations;
  Logger            logger{logfile, errfile};
  Database          database;
  std::string       build_time = logger._gen_time();
  long              msg_counter{};
  nlohmann::json    parsed;
};

class Cmds {
private:
  nlohmann::json   _reply;
  Vk_cmd_handler   _vk_handler{*this};

public:
  friend class Vk_cmd_handler;

  void init_cmds(const nlohmann::json& update);

  void stress_test(const std::string& peer_id);

  void new_post_event(const nlohmann::json& event);

  std::string crc32_cmd(_Cmd_ref args);

  std::string picture_cmd(_Cmd_ref args);

  std::string video_cmd(_Cmd_ref args);

  std::string document_cmd(_Cmd_ref args);

  std::string weather_cmd(_Cmd_ref args);

  std::string wiki_cmd(_Cmd_ref args);

  std::string laugh_cmd(_Cmd_ref args);

  std::string online_cmd(_Cmd_ref args);

  std::string kick_cmd(_Cmd_ref args);

  std::string role_cmd(_Cmd_ref args);

  std::string get_roles_cmd(_Cmd_ref args);

  std::string blacklist_cmd(_Cmd_ref args);

  std::string forbid_word_cmd(_Cmd_ref args);

  std::string repeat_cmd(_Cmd_ref args);

  std::string complete_cmd(_Cmd_ref args);

  std::string github_info_cmd(_Cmd_ref args);

  std::string genius_cmd(_Cmd_ref args);

  std::string google_cmd(_Cmd_ref args);

  std::string os_cmd(_Cmd_ref args);

  std::string reverse_cmd([[maybe_unused]] _Cmd_ref args);

  std::string currency_cmd([[maybe_unused]] _Cmd_ref args);

  std::string help_cmd([[maybe_unused]] _Cmd_ref args);

  std::string about_cmd([[maybe_unused]] _Cmd_ref args);

  std::string turn_off_cmd([[maybe_unused]] _Cmd_ref args);

  std::string stat_cmd([[maybe_unused]] _Cmd_ref args);

  std::string ping_cmd([[maybe_unused]] _Cmd_ref args);
};
} // namespace bot
