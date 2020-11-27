#include "Utils.hpp"
#include "Curl.hpp"
#include "VkAPI.hpp"
#include "Complete.hpp"

extern template class nlohmann::basic_json<>;

using nlohmann::json;
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
  json parsed =
    json::parse(cURL::requestdata("https://pelevin.gpt.dobro.ai/generate/",
      cURL::toJson({{"prompt", inputData.args}, {"length", "50"}})));

  json answer = parsed["replies"][0];
    return answer.is_null() ? "Ошибка генерации текста." : inputData.args + answer.get<std::string>();
}
