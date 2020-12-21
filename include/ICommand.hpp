#pragma once

#include <string>

namespace bot
{
/*!
 * @brief Data type to reduce number of arguments in <em><b>execute()</b></em> function.
 */
struct CommandParams
{
  const std::string args;
  const long peer_id;
};

/*!
 * @brief Command Dependencies.
 *
 * Aim of this structure - avoid creating a lot of same objects inside the commands.
 */
struct Dependencies
{
  class Repository* repository;
  class VkAPI* api;
  class Network* net;
};

/*!
 * @brief The command interface.
 */
class ICommand
{
public:
  virtual constexpr std::string_view description() const noexcept = 0;
  virtual std::string execute(const CommandParams&, const Dependencies&) = 0;
  virtual ~ICommand() = default;
};
} //namespace bot
