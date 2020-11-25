#include <chrono>

#include "Info.hpp"
#include "About.hpp"

using bot::command::AboutCommand;

std::string AboutCommand::description() const
{
  return "показать информацию о боте";
}

std::string AboutCommand::trigger() const
{
  return "+оботе";
}

AboutCommand::AboutCommand()
  : buildTime(std::string(__DATE__) + ' ' + std::string(__TIME__))
{ }

std::string AboutCommand::execute([[maybe_unused]] const CommandParams& inputData)
{
  return
    "C++ bot,\n"
    "Cурсы лежат тут: https://github.com/oxfffffe/cpp_vk_bot\n"
    "Документация: https://oxfffffe.github.io/cpp_vk_bot/\n"
    "Бота создал: @jijijijijijijijijijijijji (он)\n"
    "Версия VK API: " + info::version + '\n' +
    "Собран: " + buildTime;
}
