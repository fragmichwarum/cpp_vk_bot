#pragma once

#include <chrono>
#include <simdjson.h>

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class CurrencyCommand final : public ICommand
{
private:
  simdjson::dom::object cachedCurrency;
  simdjson::dom::parser parser;
  uint64_t lastUpdateTime;
  bool jsonWasInited = false;
  void tryCache(const uint64_t& updateInterval);

public:
  CurrencyCommand();
  const std::string execute(const CommandParams& inputData) override;
  const std::string description() const override;
  const std::string trigger() const override;
 ~CurrencyCommand()
  { }
};
} //namespace command
} //namespace bot
