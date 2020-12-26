#pragma once

#include "ICommand.hpp"

namespace bot::command
{
/*!
 * @brief Commant to stop bot.
 */
class Exit final : public ICommand
{
public:
  constexpr std::uint8_t access() const noexcept override;
  constexpr std::string_view description() const noexcept override;
  std::string execute(const CommandParams&, const Dependencies&) override;
 ~Exit() = default;
};
} //namespace bot::command
