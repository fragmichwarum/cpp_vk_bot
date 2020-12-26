#pragma once

#include "ICommand.hpp"

namespace bot::command
{
/*!
 * @brief Command that changes the name of a chat.
 */
class ChatNameSetter final : public ICommand
{
public:
  constexpr std::uint8_t access() const noexcept override;
  constexpr std::string_view description() const noexcept override;
  std::string execute(const CommandParams& inputData, const Dependencies&) override;
 ~ChatNameSetter() = default;
};
} //namespace bot::command
