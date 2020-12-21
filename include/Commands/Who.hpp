#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
/*!
 * @brief Random person picker command.
 */
class Who final : public ICommand
{
public:
  std::string execute(const CommandParams&, const Dependencies&) override;
  constexpr std::string_view description() const noexcept override;
 ~Who() = default;
};
} //namespace command
} //namespace bot
