#include "Role.hpp"
#include "Utils.hpp"

using namespace bot::command;

const std::string RoleCommand::description() const
{
  return "установить/удалить роль участника";
}

const std::string RoleCommand::trigger() const
{
  return "+роль";
}

long RoleCommand::extractId(const std::string& id)
{
  /** [@id123456789|...] */
  /**    ^        ^      */
  /**    3        9      */
  return stol(id.substr(3, 9));
}

const std::string RoleCommand::execute(const CommandParams& inputData)
{
  std::vector<std::string> args = util::split(inputData.args);

  if (args.size() != 2) {
    return "Что-то пошло не так, проверь правильность аргументов.\n"
           "+роль <айди участника> <участник|модератор|мут>.";
  }
  if (args[1] != "участник" && args[1] != "модератор" && args[1] != "мут")
  {
    return "Установлена неправильная роль. Возможные - участник, модератор, или мут.";
  }

  database.insertRole(extractId(args[0]), inputData.peer_id, args[1]);
  return "Роль успешно установлена.";
}
