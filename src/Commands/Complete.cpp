#include "Complete.hpp"
#include "Utils.hpp"
#include "Curl.hpp"

using std::string;
using bot::command::CompleteCommand;
using nlohmann::json;
using bot::cURL::to_json;

string CompleteCommand::description() const
{
  return "дополнить текст";
}

string CompleteCommand::trigger() const
{
  return "+дополни";
}

string CompleteCommand::execute(const CommandParams& inputData)
{
  if (inputData.args.empty()) {
    return util::emptyArgs();
  }
  json parsed =
    json::parse(cURL::requestdata("https://pelevin.gpt.dobro.ai/generate/",
      to_json({{"prompt", inputData.args}, {"length", "50"}})));

  json answer = parsed["replies"][0];
    return answer.is_null() ? "Ошибка генерации текста." : inputData.args + answer.get<string>();
}
