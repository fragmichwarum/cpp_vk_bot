#pragma once

#include "../ICommand.hpp"
#include "../lib/include/Json.hpp"

namespace bot
{
namespace command
{
class CurrencyCommand final : public ICommand
{
private:
  nlohmann::json cachedCurrency;
public:
  std::string execute(const CommandParams& inputData) override;
  std::string description() const;
  std::string trigger() const;
 ~CurrencyCommand()
  { }
};
} //namespace command
} //namespace bot
