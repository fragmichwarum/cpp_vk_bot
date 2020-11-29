#pragma once

#include "VkError.hpp"
#include "Invoker.hpp"
#include "ErrorLogger.hpp"
#include "../lib/include/Json.hpp"

namespace bot
{
class LongPoll
{
private:
  std::string _server;
  std::string _key;
  std::string _ts;

  std::size_t _numThreads;

  ErrorLogger _errorLogger{info::errfile};

  void _getServer();
  void _initInvoker();

  void _singleThreadProcessing(const nlohmann::json& update);
  void _multithreadProcessing(const nlohmann::json& updates);

  Invoker* const _invoker = new Invoker;

public:
  LongPoll();
  void loop();
};
}
