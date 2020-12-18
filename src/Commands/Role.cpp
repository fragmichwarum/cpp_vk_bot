#include "Repository/SQLiteRepository.hpp"
#include "Utility.hpp"
#include "Role.hpp"

std::string bot::command::Role::description() const
{
  return "установить/удалить роль участника";
}

std::string bot::command::Role::execute(const CommandParams& params, const Dependencies& deps)
{
  std::vector<std::string> args = util::split(params.args);

  if (args.size() != 2) return "Пример: +роль <айди участника> <участник|модератор|мут>.";

  if (args[1] != "участник" && args[1] != "модератор" && args[1] != "мут") {
    return "Попытка установить неправильную роль. Возможные - участник, модератор, или мут.";
  }

  deps.repository->insertRole(util::extractId(args[0]), params.peer_id, args[1]);
  return "Роль успешно установлена.";
}
