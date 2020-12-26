#pragma once

#include "ICommand.hpp"

namespace bot::command
{
/*!
 * @brief Ping checker.
 */
class Ping final : public ICommand
{
public:
  constexpr std::uint8_t access() const noexcept override;
  constexpr std::string_view description() const noexcept override;
  std::string execute(const CommandParams&, const Dependencies&) override;
 ~Ping() = default;
};
} //namespace bot::command
