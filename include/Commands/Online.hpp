#pragma once

#include "ICommand.hpp"

namespace bot::command
{
/*!
 * @brief Conversation online showing command.
 */
class Online final : public ICommand
{
public:
  constexpr std::uint8_t access() const noexcept override;
  constexpr std::string_view description() const noexcept override;
  std::string execute(const CommandParams&, const Dependencies&) override;
 ~Online() = default;
};
} //namespace bot::command
