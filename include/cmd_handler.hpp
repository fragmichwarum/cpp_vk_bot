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

using cmd_type = const cmd_args&;

class Cmd_handler;

using _Command     = std::string;
using _Description = std::string;
using _Cmd_pointer = std::string (Cmd_handler::*)(cmd_type);
using _Access      = uint8_t;
using cmds_t       = std::map<_Command, std::tuple<_Description, _Cmd_pointer, _Access>>;

extern cmds_t const vk_cmds;

static uint8_t const user      = 0x00;
static uint8_t const moderator = 0x01;
static uint8_t const creator   = 0x10;

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

  std::string crc32_cmd(cmd_type args);

  std::string picture_cmd(cmd_type args);

  std::string video_cmd(cmd_type args);

  std::string document_cmd(cmd_type args);

  std::string weather_cmd(cmd_type args);

  std::string wiki_cmd(cmd_type args);

  std::string laugh_cmd(cmd_type args);

  std::string online_cmd(cmd_type args);

  std::string kick_cmd(cmd_type args);

  std::string role_cmd(cmd_type args);

  std::string get_roles_cmd(cmd_type args);

  std::string blacklist_cmd(cmd_type args);

  std::string repeat_cmd(cmd_type args);

  std::string complete_cmd(cmd_type args);

  std::string github_info_cmd(cmd_type args);

  std::string genius_cmd(cmd_type args);

  std::string google_cmd(cmd_type args);

  std::string os_cmd(cmd_type args);

  std::string reverse_cmd([[maybe_unused]] cmd_type args);

  std::string currency_cmd([[maybe_unused]] cmd_type args);

  std::string help_cmd([[maybe_unused]] cmd_type args);

  std::string about_cmd([[maybe_unused]] cmd_type args);

  std::string turn_off_cmd([[maybe_unused]] cmd_type args);

  std::string stat_cmd([[maybe_unused]] cmd_type args);

  std::string ping_cmd([[maybe_unused]] cmd_type args);
};
} //namespace bot
