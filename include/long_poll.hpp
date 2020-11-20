#pragma once

#include <thread>

#include "vk_exception.hpp"
#include "cmd_handler.hpp"

namespace bot
{
/*!
 * @brief Class that representing interface to handling VK Long Poll.
 */
class Long_poll_handler
{
private:
  /*!
   * @brief Long Poll server address.
   */
  std::string _server;
  /*!
   * @brief Long Poll secret key.
   */
  std::string _key;
  /*!
   * @brief Long Poll ts.
   */
  std::string _ts;
  /*!
   * @brief Makes request to the Long Poll server and initialize
   * @ref _server,
   * @ref _key and
   * @ref _ts.
   */
  void _get_server();
  /*!
   * @brief An command handler class object.
   */
  Cmd_handler _handler;

public:
  /*!
   * @brief Starts endless loop, listening to Long Poll.
   */
  void loop();
};
} // namespace bot
