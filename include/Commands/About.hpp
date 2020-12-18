#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class About final : public ICommand
{
private:
  std::string buildTime;

public:
  About();
  std::string execute(const CommandParams&, const Dependencies&) override;
  std::string description() const override;
 ~About() = default;
};
} //namespace command
} //namespace bot
