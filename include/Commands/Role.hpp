#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
/*!
 * @brief Role setter/deleter.
 */
class Role final : public ICommand
{
public:
  std::string execute(const CommandParams&, const Dependencies&) override;
  constexpr std::string_view description() const noexcept override;
 ~Role() = default;
};
} //namespace command
} //namespace bot
