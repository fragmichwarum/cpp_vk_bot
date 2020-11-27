#pragma once

#include <vector>
#include <memory>

#include "ICommand.hpp"
#include "VkAPI.hpp"

//#include "Logger.hpp"
//#include "Database.hpp"

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

protected:
  void processNewPostEvent(const nlohmann::json& response);
  void processMessageEvent(const nlohmann::json& response);

public:
  void initCommand(ICommand* command);

  void tryExecute(const nlohmann::json& response);

  std::string generateHelp();

 ~Invoker()
  { }
};
}
