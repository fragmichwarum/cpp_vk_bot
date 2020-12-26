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
  const long from_id;
};
/*!
 * @brief Command Dependencies.
 *
 * Structire to avoid creating a lot of same objects inside the commands.
 */
struct Dependencies
{
  class Repository* repository;
  class JsonUtils* jsonUtils;
  class VkAPI* api;
};
/*!
 * @brief Command access levels.
 */
enum class Access : std::uint8_t
{
  user  = (1 << 1),
  admin = (1 << 2)
};
/*!
 * @brief The command interface.
 */
class ICommand
{
public:
  virtual constexpr std::uint8_t access() const noexcept = 0;
  virtual constexpr std::string_view description() const noexcept = 0;
  virtual std::string execute(const CommandParams&, const Dependencies&) = 0;
  virtual ~ICommand() = default;
};
} //namespace bot
