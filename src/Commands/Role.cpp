#include "Role.hpp"

bot::command::Role::Role()
  : repository(bot::Repository::getInstance())
{ }

const std::string bot::command::Role::description() const
{
  return "установить/удалить роль участника";
}

const std::string bot::command::Role::trigger() const
{
  return "+роль";
}

long bot::command::Role::extractId(const std::string& user)
{
  /** [@id123456789|...] */
  /**    ^        ^      */
  /**    3        9      */
  return stol(user.substr(3, 9));
}

const std::string bot::command::Role::execute(const CommandParams& params)
{
  std::vector<std::string> args = util::split(params.args);

  if (args.size() != 2) {
    return "Что-то пошло не так, проверь правильность аргументов.\n"
           "+роль <айди участника> <участник|модератор|мут>.";
  }
  if (args[1] != "участник" && args[1] != "модератор" && args[1] != "мут") {
    return "Попытка установить неправильную роль. Возможные - участник, модератор, или мут.";
  }

  repository->insertRole(extractId(args[1]), params.peer_id, args[2]);
  return "Роль успешно установлена.";
}
