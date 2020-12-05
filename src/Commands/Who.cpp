#include "Who.hpp"
#include "Utility.hpp"

const std::string bot::command::Who::description() const
{
  return "выбрать случайного участника";
}

const std::string bot::command::Who::trigger() const
{
  return "+кто";
}

const std::string bot::command::Who::execute(const CommandParams& inputData)
{
  if (inputData.args.empty()) {
    return util::emptyArgs();
  }

  std::string response = api->getConversationMembers(inputData.peer_id);

  simdjson::dom::parser parser;
  simdjson::dom::object parsed = parser.parse(response);

  if (parsed.begin().key() == "error" && parsed["error"]["error_code"].get_int64() == 917L)
  {
    return "Упс, кажется у бота нет админки.";
  }

  std::size_t size = parsed["response"]["profiles"].get_array().size();
  if (size == 0) return "Что-то пошло не так.";

  simdjson::dom::object person = parsed["response"]["profiles"].get_array().at(rand() % size);

  std::string id        (std::to_string(person["id"].get_int64()));
  std::string firstName (person["first_name"].get_c_str());
  std::string lastName  (person["last_name"].get_c_str());

  return "Хмм, я думаю, что @id" + id + '('  + firstName + ' ' + lastName + ") " + inputData.args;
}
