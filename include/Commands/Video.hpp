#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class Video final : public ICommand
{
public:
  std::string execute(const CommandParams&, const Dependencies&) override;
  std::string description() const override;
 ~Video() = default;
};
} //namespace command
} //namespace bot
