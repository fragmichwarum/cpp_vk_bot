#pragma once

#include <simdjson.h>

#include "ICommand.hpp"
#include "Info.hpp"
#include "VkAPI.hpp"
#include "EventLogger.hpp"
#include "Database.hpp"

namespace bot
{
enum EVENT
{
  MESSAGE_NEW   = 0x42d6cea9,
  WALL_POST_NEW = 0x107c64c0
};

class Invoker
{
private:
  std::vector<std::unique_ptr<ICommand>> commands;
  EventLogger eventLogger{info::logfile};
  Database database;

  void processNewPostEvent(const simdjson::dom::object& response);
  void processMessageEvent(const simdjson::dom::object& response);

public:
  void initCommand(ICommand* command);

  void tryExecute(const simdjson::dom::object& response);

  std::string generateHelp();

 ~Invoker()
  { }
};
}
