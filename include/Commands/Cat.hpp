#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class Cat final : public ICommand
{
public:
  std::string execute(const CommandParams&, const Dependencies&) override;
  std::string description() const override;
 ~Cat() = default;
};
} //namespace command
} //namespace bot
