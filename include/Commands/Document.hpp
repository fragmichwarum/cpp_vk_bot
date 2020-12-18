#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class Document final : public ICommand
{
public:
  std::string execute(const CommandParams&, const Dependencies&) override;
  std::string description() const override;
 ~Document() = default;
};
} //namespace command
} //namespace bot
