#pragma once

#include <chrono>

#include "lib/include/json.hpp"
#include "lib/include/wchar.hpp"
#include "curl.hpp"
#include "logger.hpp"
#include "split.hpp"
#include "sqlite.hpp"

/*
  Паттерны проектирования? Что это?
*/
namespace bot
{
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

class Vk_utils {
public:
  std::string       get_args       (const std::string& message);
  std::string       media_not_found(const std::string& type);
  std::string       attachment_type(const std::string& method);
  std::string       media_search   (const std::string& method, const std::string& text, const long& peer_id);
  void              message_send   (const std::string& message, const long& peer_id);
};

class Vk_cmd_handler {
private:
  Cmds*             _handler;

public:
  explicit
  Vk_cmd_handler    (Cmds&) noexcept(true);
 ~Vk_cmd_handler    ()
  { }
  std::string       empty_args() noexcept(true);
  void              init_roles(const long& peer_id);
  void              log(const std::string& message, const long& from_id);
  std::vector<long> moderators;
  std::vector<long> blacklist;
  std::vector<long> conservations;
  Logger            logger{logfile, errfile};
  Database          database;
  std::string       build_time = std::string{__DATE__} + " " + std::string{__TIME__};
  long              msg_counter{};
};

class Cmds {
private:
  nlohmann::json   _reply;
  Vk_cmd_handler   _vk_handler{*this};
  Vk_utils         _utils;

public:
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
