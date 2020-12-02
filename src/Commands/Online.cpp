//#include "Curl.hpp"
#include "Info.hpp"
#include "Online.hpp"
//#include "VkAPI.hpp"

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

  if (onlineObject.begin().key() == "error" && onlineObject["error"]["error_code"].get_int64() == 917L)
  {
    return "Упс, кажется у бота нет админки.";
  }

  std::string people = "Список людей онлайн:\n";
  for (uint8_t i = 0; i < onlineObject["response"]["profiles"].get_array().size(); i++)
  {
    simdjson::dom::object person = onlineObject["response"]["profiles"].at(i).get_object();

    std::string id        (std::to_string(person["id"].get_int64()));
    std::string firstName (person["first_name"].get_c_str());
    std::string lastName  (person["last_name"].get_c_str());

    if (person["online"].get_int64() == 1) {
      people += "@id" + id + '(' + firstName + ' ' + lastName + ")\n";
    }
  }
  return people;
  return "";
}
