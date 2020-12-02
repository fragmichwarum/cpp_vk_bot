#include "Who.hpp"
#include "Utils.hpp"
#include "Info.hpp"

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

  std::string response =
    cURL::request(cURL::appendVkUrl("messages.getConversationMembers"),
     {{ "fields",       "online"           },
      { "peer_id",      std::to_string(inputData.peer_id)},
      { "random_id",    "0"                },
      { "access_token", info::accessToken  },
      { "v",            info::version      }});

  simdjson::padded_string padded_string = response;
  simdjson::dom::parser parser;
  simdjson::dom::object parsed = parser.parse(padded_string);

  if (parsed.begin().key() == "error" && parsed["error"]["error_code"].get_int64() == 917L)
  {
    return "Упс, кажется у бота нет админки.";
  }

  std::size_t size = parsed["response"]["profiles"].get_array().size();
  if (size == 0) {
    return "Что-то пошло не так.";
  }

  simdjson::dom::object person = parsed["response"]["profiles"].get_array().at(rand() % size);

  std::string id        (std::to_string(person["id"].get_int64()));
  std::string firstName (person["first_name"].get_c_str());
  std::string lastName  (person["last_name"].get_c_str());

  return "Хмм, я думаю, что @id" + id + '('  + firstName + ' ' + lastName + ") " + inputData.args;
  return "";
}
