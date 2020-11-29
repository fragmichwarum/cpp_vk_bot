#include "Kick.hpp"
#include "Utils.hpp"
#include "VkAPI.hpp"

using bot::command::KickCommand;

const std::string KickCommand::description() const
{
  return "кикнуть юзера из беседы";
}

const std::string KickCommand::trigger() const
{
  return "+кик";
}

long KickCommand::extractId(const std::string& username)
{
  /** [@id123456789|...] */
  /**    ^        ^      */
  /**    3        9      */
  return stol(username.substr(3, 9));
}

const std::string KickCommand::execute(const CommandParams& inputData)
{
  if (inputData.args.empty()) {
    return util::emptyArgs();
  }

  return api::kick_user(inputData.peer_id - 2000000000, extractId(inputData.args));
}
