#include "Curl.hpp"
#include "Info.hpp"
#include "Online.hpp"
#include "VkAPI.hpp"

//extern template class nlohmann::basic_json<>;

using nlohmann::json;
using bot::command::OnlineCommand;

std::string OnlineCommand::description() const
{
  return "показать участников онлайн";
}

std::string OnlineCommand::trigger() const
{
  return "+онлайн";
}

std::string OnlineCommand::execute(const CommandParams& inputData)
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
  for (const auto& profile : parsed["response"]["profiles"]) {
    if (profile["online"] == 1) {
      people += "@id" + std::to_string(profile["id"].get<long>()) + '('
             +  profile["first_name"].get<std::string>() + ' '
             +  profile["last_name"].get<std::string>() + ")\n";
    }
  }
  return people;
}
