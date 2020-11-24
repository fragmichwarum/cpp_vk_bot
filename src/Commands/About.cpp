#include <chrono>

#include "About.hpp"

using std::string;
using std::to_string;
using bot::command::AboutCommand;

string AboutCommand::description() const
{
  return "показать информацию о боте";
}

string AboutCommand::trigger() const
{
  return "+оботе";
}

AboutCommand::AboutCommand()
  : buildTime(string(__DATE__) + ' ' + string(__TIME__))
{ }

string AboutCommand::execute([[maybe_unused]] const CommandParams& inputData)
{
  return
    "C++ bot,\n"
    "Cурсы лежат тут: https://github.com/oxfffffe/cpp_vk_bot\n"
    "Документация: https://oxfffffe.github.io/cpp_vk_bot/\n"
    "Бота создал: @jijijijijijijijijijijijji (он)\n"
    "Версия VK API: " + info::version + '\n' +
    "Собран: " + buildTime;
}
