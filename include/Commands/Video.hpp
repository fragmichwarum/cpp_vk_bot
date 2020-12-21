#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
/*!
 * @brief VK videos search command.
 */
class Video final : public ICommand
{
public:
  std::string execute(const CommandParams&, const Dependencies&) override;
  constexpr std::string_view description() const noexcept override;
 ~Video() = default;
};
} //namespace command
} //namespace bot
