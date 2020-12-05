 #include "About.hpp"

bot::command::About::About()
  : buildTime(std::string(__DATE__) + ' ' + std::string(__TIME__))
{ }

const std::string bot::command::About::description() const
{
  return "показать информацию о боте";
}

const std::string bot::command::About::trigger() const
{
  return "+оботе";
}

const std::string bot::command::About::execute([[maybe_unused]] const CommandParams& inputData)
{
  return
    "C++ bot,\n"
    "Cурсы лежат тут: https://github.com/oxfffffe/cpp_vk_bot\n"
    "Документация: https://oxfffffe.github.io/cpp_vk_bot/\n"
    "Бота создал: @jijijijijijijijijijijijji (он)\n"
    "Версия VK API: " + api->apiVersion + '\n' +
    "Собран: " + buildTime;
}
