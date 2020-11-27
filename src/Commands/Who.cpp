#include "Who.hpp"
#include "Curl.hpp"
#include "Utils.hpp"
#include "Info.hpp"
#include "../lib/include/Json.hpp"

extern template class nlohmann::basic_json<>;

using nlohmann::json;
using bot::command::WhoCommand;

const std::string WhoCommand::description() const
{
  return "выбрать случайного участника";
}

const std::string WhoCommand::trigger() const
{
  return "+кто";
}

const std::string WhoCommand::execute(const CommandParams& inputData)
{
  if (inputData.args.empty()) {
    return util::emptyArgs();
  }

  json parsed =
    json::parse(cURL::request(cURL::appendVkUrl("messages.getConversationMembers"),
     {{ "fields",       "online"           },
      { "peer_id",      std::to_string(inputData.peer_id)},
      { "random_id",    "0"                },
      { "access_token", info::accessToken  },
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

  return "Хмм, я думаю, что @id" + std::to_string(person["id"].get<long>()) +
         '('  + person["first_name"].get<std::string>() + ' ' + person["last_name"].get<std::string>() +
         ") " + inputData.args;
}
