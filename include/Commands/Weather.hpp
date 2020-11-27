#pragma once

#include "ICommand.hpp"
#include "Traits.hpp"
#include "../lib/include/Json.hpp"

namespace bot
{
namespace command
{
class WeatherCommand final : public ICommand
{
private:
  static traits::dictionary generateQuery(const std::string& inputData);
public:
  const std::string execute([[maybe_unused]]const CommandParams& inputData) override;
  const std::string description() const override;
  const std::string trigger() const override;
 ~WeatherCommand()
  { }
};
} //namespace command
} //namespace bot
