#pragma once

#include "lib/include/json.hpp"
#include "curl.hpp"
#include "utils.hpp"

namespace bot
{
/*!
 * @brief Class intended for working with VK API.
 */
class Vk_api
{
public:
  /*!
   * @brief Remove user from conservation.
   * @param chat_id from which the user should be removed.
   * @param user_id to be removed from chat.
   * @return Success message, error message otherwise.
   */
  std::string kick_user(const long& chat_id, const long& user_id);
  /*!
   * @brief Send message to chat.
   * @param chat_id to which the message will be sent.
   * @param user_id to be removed from chat.
   * @param options - additional options to formation of a request.
   * ### Exempli gratia:
   *
   * @code{.cpp}
   * send_message("text", 2000000008, {
   *   {"attachment", "photo..."}
   * });
   * @endcode
   */
  void send_message(const std::string& text, const long& peer_id, const std::map<std::string, std::string>& options = {});
  /*!
   * @brief Search vk media(video, pictures, docs).
   * @param Method to be evaluated
   * @param Search query.
   * @return List of VK attachments in format (photo|video|doc)<owner_id>_<id>,...
   */
  std::string media_search(const std::string& method, const std::string& text);
};
} //namespace bot
