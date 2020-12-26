#pragma once

#include "ICommand.hpp"

namespace bot::command
{
/*!
 * @brief VK documents search command.
 */
class Document final : public ICommand
{
public:
  constexpr std::uint8_t access() const noexcept override;
  constexpr std::string_view description() const noexcept override;
  std::string execute(const CommandParams&, const Dependencies&) override;
 ~Document() = default;
};
} //namespace bot::command
