#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
/*!
 * @brief VK documents search command.
 */
class Document final : public ICommand
{
public:
  std::string execute(const CommandParams&, const Dependencies&) override;
  constexpr std::string_view description() const noexcept override;
 ~Document() = default;
};
} //namespace command
} //namespace bot
