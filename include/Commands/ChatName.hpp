#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
/*!
 * @brief Command that changes the name of a chat.
 */
class ChatNameSetter final : public ICommand
{
public:
  std::string execute(const CommandParams& inputData, const Dependencies&) override;
  constexpr std::string_view description() const noexcept override;
 ~ChatNameSetter() = default;
};
} //namespace command
} //namespace bot
