#pragma once

#include "lib/include/json.hpp"
#include "curl.hpp"
#include "utils.hpp"

namespace bot
{
/*!
 * @namespace bot::info
 * @brief Namespace to store parameters needed by bot.
 *
 * All namespace members are initialized once
 * while reading JSON data from <b><em>init.json</em></b> file.
 */
namespace info
{
/*!
 * @brief VK API version.
 */
extern std::string version;
/*!
 * @brief Group access token.
 *
 * You can get it in your group settings.\n
 * Go to Settings -> API usage -> Create token.
 */
extern std::string access_token;
/*!
 * @brief User access token.
 *
 * You can get it here: https://vkhost.github.io/.
 */
extern std::string user_token;
/*!
 * @brief Group id.
 */
extern std::string group_id;
/*!
 * @brief Creator id.
 */
extern long admin_id;
/*!
 * @brief Path to log generated by @ref bot::Vk_logger class.
 */
extern std::string errfile;
/*!
 * @brief Path to error messages generated by @ref bot::Vk_logger class.
 */
extern std::string logfile;
} //namespace info

/*!
 * @brief Class intended for working with VK API.
 */
class Vk_api
{
public:
  /*!
   * @brief Remove user from conservation.
   * @param chat_id     - from which the user should be removed
   * @param user_id     - to be removed from chat
   * @return Success message, error message otherwise.
   */
  std::string kick_user(const long& chat_id, const long& user_id);
  /*!
   * @brief Send message to chat.
   * @param chat_id     - to which the message will be sent
   * @param user_id     - to be removed from chat
   * @param options     - additional options to formation of a request
   * ### Exempli gratia:
   *
   * @code{.cpp}
   *     send_message("text", 2000000008, {
   *       {"attachment", "photo..."}
   *     });
   * @endcode
   */
  void send_message(const std::string& text, const long& peer_id, const std::map<std::string, std::string>& options = {});
  /*!
   * @brief Search vk media(video, pictures, docs).
   * @param method      - Method to be evaluated
   * @param text        - Search query
   * @return List of VK attachments in format (photo|video|doc)<owner_id>_<id>,...,
   * empty string otherwise.
   */
  std::string media_search(const std::string& method, const std::string& text);
};
} //namespace bot
