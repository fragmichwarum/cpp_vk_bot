#include "VkAPI.hpp"
#include "About.hpp"

bot::command::About::About()
  : buildTime(std::string(__DATE__) + ' ' + std::string(__TIME__))
{ }

std::string bot::command::About::description() const
{
  return "показать информацию о боте";
}

std::string bot::command::About::execute([[maybe_unused]] const CommandParams& inputData, const Dependencies& deps)
{
  return
    "C++ bot,\n"
    "Cурсы лежат тут: https://github.com/oxfffffe/cpp_vk_bot\n"
    "Документация: https://oxfffffe.github.io/cpp_vk_bot/\n"
    "Бота создал: @jijijijijijijijijijijijji (он)\n"
    "Версия VK API: " + deps.api->getApiVersion() + '\n' +
    "Собран: " + buildTime;
}
