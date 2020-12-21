#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
/*!
 * @brief Command that returns common information about bot.
 */
class About final : public ICommand
{
private:
  std::string buildTime;

public:
  About();
  std::string execute(const CommandParams&, const Dependencies&) override;
  constexpr std::string_view description() const noexcept override;
 ~About() = default;
};
} //namespace command
} //namespace bot
