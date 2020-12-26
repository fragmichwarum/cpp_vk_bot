#pragma once

#include "ICommand.hpp"

namespace bot::command
{
/*!
 * @brief Hostname resolve command.
 */
class Host final : public ICommand
{
public:
  constexpr std::uint8_t access() const noexcept override;
  constexpr std::string_view description() const noexcept override;
  std::string execute(const CommandParams&, const Dependencies&) override;
 ~Host() = default;
};
} //namespace bot::command
