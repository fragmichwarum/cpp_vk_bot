#include "Repository/SQLiteRepository.hpp"
#include "Utility.hpp"
#include "Role.hpp"

constexpr std::uint8_t bot::command::Role::access() const noexcept
{
  return static_cast<std::uint8_t>(bot::Access::user);
}

constexpr std::string_view bot::command::Role::description() const noexcept
{
  return "установить/удалить роль участника";
}

std::string bot::command::Role::execute(const CommandParams& params, const Dependencies& deps)
{
  std::vector<std::string> args = util::split(params.args);

  if (args.size() != 2) return "Пример: +роль <ссылка> <участник|админ|мут>.";

  if (args[1] != "участник" && args[1] != "админ" && args[1] != "мут")
  {
    return "Попытка установить неправильную роль. Возможные - участник, админ, или мут.";
  }

  deps.repository->insertRole(util::extractId(args[0]), params.peer_id, args[1]);
  return "Роль успешно установлена.";
}
