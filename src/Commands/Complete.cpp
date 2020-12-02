//#include <simdjson.h>

#include "Utils.hpp"
//#include "Curl.hpp"
//#include "VkAPI.hpp"
#include "Complete.hpp"

using bot::command::CompleteCommand;

const std::string CompleteCommand::description() const
{
  return "дополнить текст";
}

const std::string CompleteCommand::trigger() const
{
  return "+дополни";
}

const std::string CompleteCommand::execute(const CommandParams& inputData)
{
  if (inputData.args.empty()) {
    return util::emptyArgs();
  }
  std::string response =
    cURL::requestdata("https://pelevin.gpt.dobro.ai/generate/",
    cURL::toJson({{"prompt", inputData.args}, {"length", "50"}}));

  simdjson::padded_string padded_string = response;
  simdjson::dom::parser parser;
  simdjson::dom::object AIObject = parser.parse(padded_string);


  return AIObject["replies"].at(0).is_null()
    ? "Ошибка генерации текста."
    : inputData.args + std::string{AIObject["replies"].at(0).get_c_str()};
}
