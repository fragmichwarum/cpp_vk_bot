#pragma once

#include "VkError.hpp"
#include "Invoker.hpp"
#include "../lib/include/Json.hpp"

namespace bot
{
class LongPoll final
{
private:
  std::string _server;
  std::string _key;
  std::string _ts;

  std::size_t _numThreads;

  void _get_server();
  void _init_invoker();

  void _singleThreadProcessing(const nlohmann::json& update);
  void _multithreadProcessing(const nlohmann::json& updates);

  Invoker* const _invoker = new Invoker;

public:
  LongPoll();
  void loop();
};
}
