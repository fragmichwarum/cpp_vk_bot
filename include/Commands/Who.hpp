#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class Who final : public ICommand
{
public:
  std::string execute(const CommandParams&, const Dependencies&) override;
  std::string description() const override;
 ~Who() = default;
};
} //namespace command
} //namespace bot
