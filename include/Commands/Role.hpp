#pragma once

#include "ICommand.hpp"

namespace bot::command
{
/*!
 * @brief Role setter/deleter.
 */
class Role final : public ICommand
{
public:
  constexpr std::uint8_t access() const noexcept override;
  constexpr std::string_view description() const noexcept override;
  std::string execute(const CommandParams&, const Dependencies&) override;
 ~Role() = default;
};
} //namespace bot::command
