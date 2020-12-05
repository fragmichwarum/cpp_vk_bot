#include "Utility.hpp"
#include "Complete.hpp"

const std::string bot::command::Complete::description() const
{
  return "дополнить текст";
}

const std::string bot::command::Complete::trigger() const
{
  return "+дополни";
}

const std::string bot::command::Complete::execute(const CommandParams& inputData)
{
  if (inputData.args.empty()) {
    return util::emptyArgs();
  }

  std::string response =
    net->requestdata("https://pelevin.gpt.dobro.ai/generate/",
    net->toJson({{"prompt", inputData.args}, {"length", "50"}}));

  simdjson::dom::parser parser;
  simdjson::dom::object AIObject = parser.parse(response);

  return AIObject["replies"].at(0).is_null()
    ? "Ошибка генерации текста."
    : inputData.args + std::string{AIObject["replies"].at(0).get_c_str()};
}
