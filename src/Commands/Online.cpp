#include "Online.hpp"

const std::string bot::command::Online::description() const
{
  return "показать участников онлайн";
}

const std::string bot::command::Online::trigger() const
{
  return "+онлайн";
}

const std::string bot::command::Online::execute(const CommandParams& params)
{
  simdjson::dom::parser parser;
  simdjson::dom::object onlineObject = parser.parse(api->getConversationMembers(params.peer_id));

  if (onlineObject.begin().key() == "error" && onlineObject["error"]["error_code"].get_int64() == 917L)
  {
    return "Упс, кажется у бота нет админки.";
  }

  std::string people = "Список людей онлайн:\n";
  for (const simdjson::dom::element& profile : onlineObject["response"]["profiles"].get_array())
  {
    if (profile["online"].get_int64() == 1)
    {
      std::string id        (std::to_string(profile["id"].get_int64()));
      std::string firstName (profile["first_name"].get_c_str());
      std::string lastName  (profile["last_name"].get_c_str());
      people += "@id" + id + '(' + firstName + ' ' + lastName + ")\n";
    }
  }
  return people;
}
