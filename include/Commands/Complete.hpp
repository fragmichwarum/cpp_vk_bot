#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class Complete final : public ICommand
{
private:
  Network* net = Network::getInstance();

public:
  const std::string execute([[maybe_unused]]const CommandParams& inputData) override;
  const std::string description() const override;
  const std::string trigger() const override;
 ~Complete()
  { }
};
} //namespace command
} //namespace bot
