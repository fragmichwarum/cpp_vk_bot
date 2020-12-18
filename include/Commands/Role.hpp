#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class Role final : public ICommand
{
public:
  std::string execute(const CommandParams&, const Dependencies&) override;
  std::string description() const override;
 ~Role() = default;
};
} //namespace command
} //namespace bot
