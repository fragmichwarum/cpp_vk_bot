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
public:
  /*!
   * @brief The MessageEventHandler constructor.
   *
   * Initializes @ref commandCollection_.
   */
  explicit
  MessageEventHandler();
  /*!
   * @brief Destructor marked as <em>default</em>.
   *
   * It must be defined in place, when <em>std::unique_ptr</em> pointed-to type is complete.
   */
 ~MessageEventHandler();
  /*!
   * @brief Try match command by trigger.
   *
   * Execute the command if first word was matched with any trigger, do nothing otherwise.
   * @param JSON update.
   */
  void process(std::string_view message, long peer_id, long from_id);

private:
  std::unique_ptr<class VkAPI> api;
  std::unique_ptr<class Logger> logger;

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
  /*!
   * @brief checkUserRole
   * @param peer_id
   */
  bool isAdmin(long from_id, long peer_id);

  void tryProcessCommand(std::string_view message, long peer_id, long from_id);
};
} //namespace bot
