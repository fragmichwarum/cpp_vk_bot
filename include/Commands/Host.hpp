#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class Host final : public ICommand
{
public:
  std::string execute(const CommandParams&, const Dependencies&) override;
  std::string description() const override;
 ~Host() = default;
};
} //namespace command
} //namespace bot
