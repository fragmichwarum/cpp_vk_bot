#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
class GithubCommand final : public ICommand
{
private:
  struct non_alpha;
  inline bool is_latin(const std::string& text);
  static std::string searchRepos(uint8_t reposCount, const std::string& user, const std::string& repo = 0);
  static std::string getUserRepo(const std::string& user, const std::string& repo);
  static std::string getUserRepos(const std::string& user);

public:
  std::string execute([[maybe_unused]]const CommandParams&) override;
  std::string description() const override;
  std::string trigger() const override;
 ~GithubCommand()
  { }
};
} //namespace command
} //namepsace bot
