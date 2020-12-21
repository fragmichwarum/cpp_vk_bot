#include <simdjson.h>

#include "Network.hpp"
#include "Utility.hpp"
#include "Complete.hpp"

constexpr std::string_view bot::command::Complete::description() const noexcept
{
  return "дополнить текст";
}

std::string bot::command::Complete::execute(const CommandParams& inputData, const Dependencies& deps)
{
  if (inputData.args.empty()) {
    return util::emptyArgs();
  }

  std::string_view response =
    deps.net->requestdata("https://pelevin.gpt.dobro.ai/generate/",
    util::toJson({{"prompt", inputData.args}, {"length", "50"}}));

  simdjson::dom::parser parser;
  simdjson::dom::object AIObject = parser.parse(response);

  return AIObject["replies"].at(0).is_null()
    ? "Ошибка генерации текста."
    : inputData.args + std::string{AIObject["replies"].at(0).get_c_str()};
}
