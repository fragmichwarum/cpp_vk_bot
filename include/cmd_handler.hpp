#pragma once

#include "database.hpp"
#include "logger.hpp"
#include "vk_api.hpp"

#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wcomment"
#endif
/*!
 * @mainpage Detailed description of https://github.com/oxfffffe/cpp_vk_bot
 *
 * ## How to start?
 *
 * First of all, you have take a look in <b><em>init.json</em></b> file.
 *
 * ```
 * {
 *     "token": {
 *         "access_token": "Access token of your group",
 *         "user_token": "Access token of your user"
 *     },
 *     "group_id": "ID of your group",
 *     "admin_id": Your user ID,
 *     "path": {
 *         "log": "The file where the log will be stored",
 *         "err": "The file where the error log will be stored"
 *     },
 *     "api_v": "5.124"
 * }
 * ```
 *
 * ## Build
 *
 * There are multiple options to choose.
 *
 * ### QMake
 * ```
 * qmake vk.pro
 * make (optional) -j <the number of your CPU cores>
 * ```
 * ### GCC
 * ```
 * g++ -c -pipe -Wall -Os -std=gnu++1z -fPIC -I. -Iinclude lib/src/* src/* && g++ -Wl,-O1 -o vk *.o -lcurl -lsqlite3 -lpthread && rm *.o -rf
 * ```
 * ### Clang
 * ```
 * clang++ -c -pipe -Wall -Os -std=gnu++1z -fPIC -I. -Iinclude lib/src/* src/* && clang++ -Wl,-O1 -o vk *.o -lcurl -lsqlite3 -lpthread && rm *.o -rf
 * ```
 *
 * ## Used libraries
 *  - sqlite3
 *  - libcurl
 *  - @ref nlohmann::json "Nlohmann JSON"
 *
 * ## Reference
 *
 * - Namespace @ref bot "bot":
 *   - Classes:
 *     - @ref bot::Cmd_handler "Cmd_handler"
 *     - @ref bot::Cmd_traits "Cmd_traits"
 *     - @ref bot::Database "Database"
 *     - @ref bot::Long_poll_handler "Long_poll_handler"
 *     - @ref bot::Vk_api "Vk_api"
 *     - @ref bot::Vk_exception "Vk_exception"
 *     - @ref bot::Vk_logger "Vk_logger"
 *   - Nested namespaces:
 *     - @ref bot::cURL "cURL"
 *     - @ref bot::util "util"
 *     - @ref bot::info "info"
 *   - Type definitions:
 *     - @ref bot::cmd_type "cmd_type"
 *   - Enumerations:
 *     - @ref bot::LOGTYPE "LOGTYPE"
 */
#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
#  pragma GCC diagnostic pop
#endif

/*!
 * @namespace bot
 * @brief main namespace
 */
namespace bot
{
/*!
 * @brief typedef struct for command arguments.
 */
typedef struct {
  std::string message;
  long peer_id;
} cmd_args;

/*!
 *  @brief Alias for const reference to cmd_args struct.
 */
using cmd_type = const cmd_args&;

class Cmd_handler;

/*!
 * @brief Class that repersents type traits for Cmd_handler.
 */
class Cmd_traits
{
public:
  /*!
   * @brief Alias for function pointer stored in @ref Cmd_handler class.
   */
  using cmd_pointer = std::string(Cmd_handler::*)(cmd_type);
  /*!
   * @brief Alias for command name.
   */
  using command = std::string;
  /*!
   * @brief Alias for command description. Needed by @ref Cmd_handler::help_cmd(@ref cmd_type);
   */
  using description = std::string;
  /*!
   * @brief Alias for access modifier.
   */
  using access = uint8_t;
  /*!
   * @brief Alias for second type of @ref cmds_t.
   */
  using command_params = std::tuple<description, cmd_pointer, access>;
  /*!
   * @brief Alias for std::map. This container stores an enumeration of all bot commands.
   */
  using cmds_t = std::map<command, command_params>;
};

/*!
 * @brief Class representing the interface for processing commands.
 * - This class has no constructors with parameters and no static fields for the
 *   <b><em>message</em></b>, <b><em>from_id</em></b> and <b><em>peer_id</em></b> due to the fact that
 *   @ref bot::Long_poll_handler works with threads.
 */
class Cmd_handler
{
public:
  /*!
   * @brief User access modifier.
   *
   * @note Users with @ref user access
   * modified able to execute any command except the
   * commands with
   * @ref moderator and
   * @ref creator access modifiers.
   */
  static constexpr uint8_t const user = 0x00;
  /*!
   * @brief Moderator access modifier.
   *
   * @note Users with moderator access
   * modifier able to execute\n
   * @ref Cmd_handler::kick_cmd(@ref cmd_type)\n
   * @ref Cmd_handler::reverse_cmd(@ref cmd_type)\n
   * @ref Cmd_handler::kick_cmd(@ref cmd_type)\n
   * @ref Cmd_handler::blacklist_cmd(@ref cmd_type).
   */
  static constexpr uint8_t const moderator = 0x01;
  /*!
   * @brief Creator access modifier.
   *
   * @note Be careful. Users with creator access
   * modifier able to execute\n
   * @ref Cmd_handler::repeat_cmd(@ref cmd_type)\n
   * @ref Cmd_handler::os_cmd(@ref cmd_type)\n
   * @ref Cmd_handler::turn_off_cmd(@ref cmd_type)
   */
  static constexpr uint8_t const creator = 0x10;
  /*!
   * @brief Enumeration of all bot commands with their
   * @ref Cmd_traits::command_params "parameters".
   */
  static Cmd_traits::cmds_t const vk_cmds;

private:
  /*!
   * @brief Message reply stored in @ref nlohmann::json "JSON".
   */
  nlohmann::json _reply;
  /*!
   * @brief VK API handler.
   */
  Vk_api _api;
  /*!
   * @brief SQLite3 database storing roles.
   */
  Database _database;
  /*!
   * @brief Logger for incoming events.
   */
  Vk_logger _logger{info::logfile, info::errfile};
  /*!
   * @brief Count of processed commands.
   * Needed by @ref bot::Cmd_handler::stat_cmd.
   */
  long _msg_counter = 0;
  /*!
   * @brief Bot build time.
   * Needed by @ref bot::Cmd_handler::stat_cmd.
   */
  std::string const _build_time = std::string{__DATE__} + " " + std::string{__TIME__};

public:
  /*!
   * @brief Incoming event initial function.
   * @param An @ref nlohmann::basic_json "JSON" update object.
   */
  void init_cmds(const nlohmann::json& update);
  /*!
   * @brief Function, that notifies users about a new post in the group.
   * @param An @ref nlohmann::basic_json "JSON" update object.
   */
  void new_post_event(const nlohmann::json& event);
  /*!
   * @brief CRC32 command.
   * @param A @ref Cmd_traits::cmds_t "command type".
   * @return CRC32-hash of string, generated by @ref unsigned long crc32gen(const char*);
   */
  std::string crc32_cmd(cmd_type args);
  /*!
   * @brief Command, that searches for pictures.
   *
   * Calls @ref bot::Vk_api::media_search.
   * @param A @ref Cmd_traits::cmds_t "command type".
   * @return Empty string on success, error message otherwise.
   */
  std::string picture_cmd(cmd_type args);
  /*!
   * @brief Command, that searches for videos.
   *
   * Calls @ref bot::Vk_api::media_search.
   * @param A @ref Cmd_traits::cmds_t "command type".
   * @return Empty string on success, error message otherwise.
   */
  std::string video_cmd(cmd_type args);
  /*!
   * @brief Command, that searches for documents.
   *
   * Calls @ref bot::Vk_api::media_search.
   * @param A @ref Cmd_traits::cmds_t "command type".
   * @return Empty string on success, error message otherwise.
   */
  std::string document_cmd(cmd_type args);
  /*!
   * @brief Command showing the weather.
   * @param A @ref Cmd_traits::cmds_t "command type".
   * @return Formatted result of http://api.openweathermap.org work.
   */
  std::string weather_cmd(cmd_type args);
  /*!
   * @brief Command searching info in Wiki.
   * @param A @ref Cmd_traits::cmds_t "command type".
   * @return Result of https://ru.wikipedia.org/w/api.php work.
   */
  std::string wiki_cmd(cmd_type args);
  /*!
   * @brief Command that generates random laugh.
   * @param A @ref Cmd_traits::cmds_t "command type".
   * @return A sequence of characters similar(almost) to laughter.
   */
  std::string laugh_cmd(cmd_type args);
  /*!
   * @brief Command for moderators that shows online users in current chat.
   * @param A @ref Cmd_traits::cmds_t "command type".
   * @return List of online users.
   */
  std::string online_cmd(cmd_type args);
  /*!
   * @brief Command for moredators that removes user from chat.
   * @param A @ref Cmd_traits::cmds_t "command type".
   * @return Farewell message, error message otherwise.
   */
  std::string kick_cmd(cmd_type args);
  /*!
   * @brief Command for moredators that set up role for user.
   * @param A @ref Cmd_traits::cmds_t "command type".
   * @return Success message, error message otherwise.
   */
  std::string role_cmd(cmd_type args);
  /*!
   * @brief Command that shows users roles in current chat.
   * @param A @ref Cmd_traits::cmds_t "command type".
   * @return List of roles.
   */
  std::string get_roles_cmd(cmd_type args);
  /*!
   * @brief Command for moderators that adds a user to blacklist.
   * @param A @ref Cmd_traits::cmds_t "command type".
   * @note Command adds a user to LOCAL database blacklist, not blacklist of VK chat!
   * @return List of roles.
   */
  std::string blacklist_cmd(cmd_type args);
  /*!
   * @brief Command for creator that resend command argument.
   * @param A @ref Cmd_traits::cmds_t "command type".
   * @return Whole command argument.
   */
  std::string repeat_cmd(cmd_type args);
  /*!
   * @brief Text complement command.
   * @param A @ref Cmd_traits::cmds_t "command type".
   * @return Result of https://pelevin.gpt.dobro.ai/generate/ work.
   */
  std::string complete_cmd(cmd_type args);
  /*!
   * @brief Command that searches various information about GitHub users.
   * @param A @ref Cmd_traits::cmds_t "command type".
   * @return Result of https://api.github.com/ work.
   */
  std::string github_info_cmd(cmd_type args);
  /*!
   * @brief Command that searches music tracks & lyrics on Genius.
   * @param A @ref Cmd_traits::cmds_t "command type".
   * @return Result of https://api.genius.com/ work.
   */
  std::string genius_cmd(cmd_type args);
  /*!
   * @brief Command that redirect forwarded message to Google :).
   * @param A @ref Cmd_traits::cmds_t "command type".
   * @return Link to Google.
   */
  std::string google_cmd(cmd_type args);
  /*!
   * @brief Command for creator that executes bash command.
   * @param A @ref Cmd_traits::cmds_t "command type".
   * @return Bash output.
   */
  std::string os_cmd(cmd_type args);
  /*!
   * @brief Command for moderators(for some reasons) that reverses a string.
   * @param A @ref Cmd_traits::cmds_t "command type".
   * @return Reversed string.
   */
  std::string reverse_cmd([[maybe_unused]] cmd_type args);
  /*!
   * @brief Command that returns list of currencies with their traits.
   * @param A @ref Cmd_traits::cmds_t "command type".
   * @return Result of https://www.cbr-xml-daily.ru/daily_json.js work.
   */
  std::string currency_cmd([[maybe_unused]] cmd_type args);
  /*!
   * @brief Command that shows help.
   * @param A @ref Cmd_traits::cmds_t "command type".
   * @return List of commands in chunks of 15.
   */
  std::string help_cmd([[maybe_unused]] cmd_type args);
  /*!
   * @brief Command that shows info about bot.
   * @param A @ref Cmd_traits::cmds_t "command type".
   * @return GitHub repo, Author name, VK API version and build time.
   */
  std::string about_cmd([[maybe_unused]] cmd_type args);
  /*!
   * @brief Command for creator only, that shows info about bot.
   * @param A @ref Cmd_traits::cmds_t "command type".
   * @return empty string.
   */
  std::string turn_off_cmd([[maybe_unused]] cmd_type args);
  /*!
   * @brief Command that shows bot statistics.
   * @param A @ref Cmd_traits::cmds_t "command type".
   * @return Total memory, memory used by bot, uptime, count of used threads and count of processed commands.
   */
  std::string stat_cmd([[maybe_unused]] cmd_type args);
  /*!
   * @brief Command that shows delay when accessing api.vk.com.
   * @param A @ref Cmd_traits::cmds_t "command type".
   * @return 10 tries to ping api.vk.com and average delay.
   */
  std::string ping_cmd([[maybe_unused]] cmd_type args);
  /*!
   * @brief Command for admin only to download file.
   * @param A @ref Cmd_traits::cmds_t "command type".
   * @return Empty string on success, error message othrewise.
   */
  std::string download_cmd(cmd_type args);
  /*!
   * @brief Command looking for random cat image.
   * @param A @ref Cmd_traits::cmds_t "command type".
   * @return Empty string on success, error message othrewise.
   */
  std::string cat_cmd(cmd_type args);
};
} //namespace bot
