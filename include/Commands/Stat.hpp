#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
/*!
 * @brief Statistics showing command.
 */
class Stat final : public ICommand
{
public:
  std::string execute(const CommandParams&, const Dependencies&) override;
  constexpr std::string_view description() const noexcept override;
 ~Stat() = default;
};
} //namespace command
} //namespace bot
