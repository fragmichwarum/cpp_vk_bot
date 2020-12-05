#pragma once

#include "Logger.hpp"
#include "ICommand.hpp"

namespace bot
{
class MessageEventHandler
{
private:
  VkAPI* api = VkAPI::getInstance();
  Logger logger{api->logPath, api->errPath};

  std::vector<std::shared_ptr<ICommand>> commandCollection;
  void processLogging(const std::string& message);

public:
  void tryProcessMessage(const simdjson::dom::object& update);
  void addCommand(ICommand* command);
};
} //namespace bot
