#pragma once

#include <simdjson.h>

#include "ICommand.hpp"

namespace bot
{
/*!
 * @brief Incoming messages handler.
 */
class MessageEventHandler
{
private:
  static class VkAPI* api;
  static class Logger* logger;

  /*!
   * @brief List of commands.
   */
  std::unordered_map<std::string, std::shared_ptr<ICommand>> commandCollection;

  /*!
   * @brief Write and print message.
   */
  void processLogging_(std::string_view message);
  /*!
   * @brief Generate help from <em><b>description()</b></em> methods of Command classes.
   * @param peer_id     - destination.
   */
  void processHelp_(const long& peer_id);

public:
  /*!
   * @brief The MessageEventHandler constructor.
   *
   * Initializes @ref commandCollection_.
   */
  MessageEventHandler();
  /*!
   * @brief Iterate through @ref commandCollection_ and try match command trigger.
   * Execute the command if successful, otherwise do nothing.
   * @param JSON update.
   */
  void tryProcessMessage(const simdjson::dom::object& update);

 ~MessageEventHandler();
};
} //namespace bot
