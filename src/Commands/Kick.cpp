#include "Kick.hpp"
#include "Utils.hpp"

using std::string;
using bot::command::KickCommand;

string KickCommand::description() const
{
  return "кикнуть юзера из беседы";
}

string KickCommand::trigger() const
{
  return "+кик";
}

long KickCommand::extractId(const string& username)
{
  /** [@id123456789|...] */
  /**     ^       ^      */
  /**     3       9      */
  return stol(username.substr(3, 9));
}

string KickCommand::execute(const CommandParams& inputData)
{
  if (inputData.args.empty()) {
    return util::emptyArgs();
  }

  return vkapi::kick_user(inputData.peer_id - 2000000000, extractId(inputData.args));
}
