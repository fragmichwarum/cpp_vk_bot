#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class Role final : public ICommand
{
private:
  Repository* repository;
  static long extractId(const std::string& user);

public:
  Role();
  const std::string execute([[maybe_unused]]const CommandParams&) override;
  const std::string description() const override;
  const std::string trigger() const override;
  ~Role()
  { }
};
}
}