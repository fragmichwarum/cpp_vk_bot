#include "Curl.hpp"
#include "Online.hpp"

using std::string;
using std::to_string;
using nlohmann::json;
using bot::command::OnlineCommand;

string OnlineCommand::description() const
{
  return "показать участников онлайн";
}

string OnlineCommand::trigger() const
{
  return "+онлайн";
}

string OnlineCommand::execute(const CommandParams& inputData)
{
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
  string people = "Список людей онлайн:\n";
  for (const auto& profile : parsed["response"]["profiles"]) {
    if (profile["online"] == 1) {
      people += "@id" + to_string(profile["id"].get<long>()) + '('
             +  profile["first_name"].get<string>() + ' '
             +  profile["last_name"].get<string>() + ")\n";
    }
  }
  return people;
}
