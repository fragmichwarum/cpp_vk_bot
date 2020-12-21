#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
/*!
 * @brief Command looking currencies.
 */
class Currency final : public ICommand
{
public:
  std::string execute(const CommandParams&, const Dependencies&) override;
  constexpr std::string_view description() const noexcept override;
 ~Currency() = default;
};
} //namespace command
} //namespace bot
