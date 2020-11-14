#pragma once

#include "database.hpp"
#include "logger.hpp"
#include "vk_api.hpp"

namespace bot
{
typedef struct {
  std::string message;
  long peer_id;
} cmd_args;

using _Cmd_ref = const cmd_args&;

class Cmd_handler;

using command     = std::string;
using description = std::string;
using cmd_pointer = std::string (Cmd_handler::*)(_Cmd_ref);
using access      = uint8_t;
using cmds_t      = std::map<command, std::tuple<description, cmd_pointer, access>>;

extern cmds_t const vk_cmds;

class Cmd_handler
{
private:
  nlohmann::json    _reply;
  Vk_api            _api;
  Database          _database;
  Logger            _logger{logfile, errfile};
  void              _log(const std::string& message, const long& from_id);
  void              _init_roles(const long& peer_id);
  long              _msg_counter{};
  std::string const _build_time = std::string{__DATE__} + " " + std::string{__TIME__};

public:
  void init_cmds(const nlohmann::json& update);

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
} //namespace bot
