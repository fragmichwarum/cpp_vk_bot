#pragma once

#include <unordered_map>
#include <memory>

#include "ICommand.hpp"

namespace simdjson::dom
{
class object;
}

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
  std::unordered_map<std::string_view, std::unique_ptr<ICommand>> commandCollection;

  /*!
   * @brief Write and print message.
   */
  void processLogging(std::string_view message);
  /*!
   * @brief Generate help from <em><b>description()</b></em> methods of Command classes.
   * @param peer_id     - destination.
   */
  void processHelp(long peer_id);

public:
  /*!
   * @brief The MessageEventHandler constructor.
   *
   * Initializes @ref commandCollection_.
   */
  MessageEventHandler();
  /*!
   * @brief Try match command by trigger.
   *
   * Execute the command if first word was matched with any trigger, do nothing otherwise.
   * @param JSON update.
   */
//  void tryProcessMessage(const simdjson::dom::object& update);
  void tryProcessMessage(std::string_view message, long peer_id);

 ~MessageEventHandler();
};
} //namespace bot
