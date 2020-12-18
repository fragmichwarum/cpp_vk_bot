#pragma once

#include <simdjson.h>

#include "ICommand.hpp"

namespace bot
{
class MessageEventHandler
{
private:
  static class VkAPI* api_;
  static class Logger* logger_;
  static class Network* net_;
  static class SQLiteRepository* repository_;

  std::unordered_map<std::string, std::shared_ptr<ICommand>> commandCollection_;

  void processLogging_(const std::string& message);
  void processHelp_(const long& peer_id);

public:
  MessageEventHandler();
  void tryProcessMessage(const simdjson::dom::object& update);

 ~MessageEventHandler();
};
} //namespace bot
