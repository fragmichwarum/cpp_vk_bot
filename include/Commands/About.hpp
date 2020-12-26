#pragma once

#include "ICommand.hpp"

namespace bot::command
{
/*!
 * @brief Command that returns common information about bot.
 */
class About final : public ICommand
{
public:
  constexpr std::uint8_t access() const noexcept override;
  constexpr std::string_view description() const noexcept override;
  std::string execute(const CommandParams&, const Dependencies&) override;
 ~About() = default;
};
} //namespace bot::command
