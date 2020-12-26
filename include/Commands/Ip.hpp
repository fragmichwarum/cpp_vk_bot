#pragma once

#include "ICommand.hpp"

namespace bot::command
{
/*!
 * @brief Ip address resolve command.
 */
class Ip final : public ICommand
{
public:
  constexpr std::uint8_t access() const noexcept override;
  constexpr std::string_view description() const noexcept override;
  std::string execute(const CommandParams&, const Dependencies&) override;
 ~Ip() = default;
};
} //namespace bot::command
