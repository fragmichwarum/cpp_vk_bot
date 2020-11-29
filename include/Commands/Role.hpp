#pragma once

#include "ICommand.hpp"
#include "Database.hpp"

namespace bot
{
namespace command
{
class RoleCommand final : public ICommand
{
private:
  Database database;
  static long extractId(const std::string& id);

public:
  const std::string execute(const CommandParams& inputData) override;
  const std::string description() const override;
  const std::string trigger() const override;
 ~RoleCommand()
  { }
};
} //namespace command
} //namespace bot
