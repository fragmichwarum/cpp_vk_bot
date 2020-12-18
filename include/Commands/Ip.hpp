#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class Ip final : public ICommand
{
public:
  std::string execute(const CommandParams&, const Dependencies&) override;
  std::string description() const override;
 ~Ip() = default;
};
} //namespace command
} //namespace bot

