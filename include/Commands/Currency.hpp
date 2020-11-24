#pragma once

#include <chrono>

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class CurrencyCommand final : public ICommand
{
private:
  nlohmann::json cachedCurrency;
  uint64_t lastUpdateTime;
  void tryCache(const uint64_t& updateInterval);

public:
  CurrencyCommand();
  std::string execute(const CommandParams& inputData) override;
  std::string description() const override;
  std::string trigger() const override;
 ~CurrencyCommand()
  { }
};
} //namespace command
} //namespace bot
