#include "Who.hpp"
#include "Curl.hpp"
#include "Utils.hpp"

using namespace bot::util;
using bot::command::WhoCommand;
using std::string;
using std::to_string;
using nlohmann::json;

string WhoCommand::description() const
{
  return "выбрать случайного участника";
}

string WhoCommand::trigger() const
{
  return "+кто";
}

string WhoCommand::execute(const CommandParams& inputData)
{
  if (inputData.args.empty()) {
    return emptyArgs();
  }

  json parsed =
    json::parse(cURL::request(cURL::append_vkurl("messages.getConversationMembers"),
     {{ "fields",       "online"           },
      { "peer_id",      to_string(inputData.peer_id)},
      { "random_id",    "0"                },
      { "access_token", info::access_token },
      { "v",            info::version      }}));

  if (not parsed["error"].is_null() &&
          parsed["error"]["error_code"] == 917L)
  {
    return "Упс, кажется у бота нет админки.";
  }

  long size = parsed["response"]["profiles"].size();
  if (size == 0) {
    return "Что-то пошло не так.";
  }

  json person = parsed["response"]["profiles"][rand() % size];

  return "Хмм, я думаю, что @id" + to_string(person["id"].get<long>()) +
         '('  + person["first_name"].get<string>() + ' ' + person["last_name"].get<string>() +
         ") " + inputData.args;
}
