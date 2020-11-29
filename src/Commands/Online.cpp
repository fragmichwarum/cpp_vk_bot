#include "Curl.hpp"
#include "Info.hpp"
#include "Online.hpp"
#include "VkAPI.hpp"

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
  std::string response =
    cURL::request(cURL::appendVkUrl("messages.getConversationMembers"),
     {{ "fields",       "online"           },
      { "peer_id",      std::to_string(inputData.peer_id)},
      { "random_id",    "0"                },
      { "access_token", info::accessToken  },
      { "v",            info::version      }});

  simdjson::padded_string padded_string = response;
  simdjson::dom::parser parser;
  simdjson::dom::object onlineObject = parser.parse(padded_string);

  if (onlineObject["response"].is_null())
  {
    return "Упс, кажется у бота нет админки.";
  }

  std::string people = "Список людей онлайн:\n";
  for (uint8_t i = 0; i < onlineObject["response"]["profiles"].get_array().size(); i++) {
    simdjson::dom::object person = onlineObject["response"]["profiles"].at(i);

    if (person["online"].get_int64() == 1) {
      people += "@id" + std::to_string(person["id"].get_int64()) + '('
             +  std::string{person["first_name"].get_c_str()} + ' '
             +  std::string{person["last_name"].get_c_str()} + ")\n";
    }
  }
  return people;
  return "";
}
