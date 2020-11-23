#pragma once

#include "../ICommand.hpp"
#include "../VkAPI.hpp"

namespace bot
{
namespace command
{
class WeatherCommand final : public ICommand
{
private:
  static nlohmann::json executeQuery(const std::string& inputData);
public:
  std::string execute([[maybe_unused]]const CommandParams& inputData) override;
  std::string description() const override;
  std::string trigger() const override;
 ~WeatherCommand()
  { }
};
} //namespace command
} //namespace bot
