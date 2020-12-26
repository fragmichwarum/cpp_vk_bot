#pragma once

#include "ICommand.hpp"

namespace bot::command
{
/*!
 * @brief Statistics showing command.
 */
class Stat final : public ICommand
{
public:
  constexpr std::uint8_t access() const noexcept override;
  constexpr std::string_view description() const noexcept override;
  std::string execute(const CommandParams&, const Dependencies&) override;
 ~Stat() = default;
};
} //namespace bot::command
