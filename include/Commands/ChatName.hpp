#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class ChatNameSetter final : public ICommand
{
public:
  std::string execute(const CommandParams& inputData, const Dependencies&) override;
  std::string description() const override;
 ~ChatNameSetter() = default;
};
} //namespace command
} //namespace bot
