#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class Stat final : public ICommand
{
public:
  std::string execute(const CommandParams&, const Dependencies&) override;
  std::string description() const override;
 ~Stat() = default;
};
} //namespace command
} //namespace bot
