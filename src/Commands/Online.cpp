#include "Curl.hpp"
#include "Info.hpp"
#include "Online.hpp"
#include "VkAPI.hpp"

extern template class nlohmann::basic_json<>;

using nlohmann::json;
using bot::command::OnlineCommand;

const std::string OnlineCommand::description() const
{
  return "показать участников онлайн";
}

const std::string OnlineCommand::trigger() const
{
  return "+онлайн";
}

const std::string OnlineCommand::execute(const CommandParams& inputData)
{
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
  std::string people = "Список людей онлайн:\n";
  for (uint8_t i = 0; i < parsed["response"]["profiles"].size(); i++) {
    json person = parsed["response"]["profiles"][i];

    if (person["online"] == 1) {
      people += "@id" + std::to_string(person["id"].get<long>()) + '('
             +  person["first_name"].get<std::string>() + ' '
             +  person["last_name"].get<std::string>() + ")\n";
    }
  }
  return people;
}
