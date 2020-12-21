#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
/*!
 * @brief VK pictures search command.
 */
class Picture final : public ICommand
{
public:
  std::string execute(const CommandParams&, const Dependencies&) override;
  constexpr std::string_view description() const noexcept override;
 ~Picture() = default;
};
} //namespace command
} //namespace bot
