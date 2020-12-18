#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class Currency final : public ICommand
{
public:
  std::string execute(const CommandParams&, const Dependencies&) override;
  std::string description() const override;
 ~Currency() = default;
};
} //namespace command
} //namespace bot
