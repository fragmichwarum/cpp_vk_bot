#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class Online final : public ICommand
{
public:
  std::string execute(const CommandParams&, const Dependencies&) override;
  std::string description() const override;
 ~Online() = default;
};
} //namespace command
} //namespace bot
