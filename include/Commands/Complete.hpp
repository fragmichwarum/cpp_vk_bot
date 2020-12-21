#pragma once

#include "ICommand.hpp"

namespace bot
{
namespace command
{
/*!
 * @brief <em><b>https://pelevin.gpt.dobro.ai</b></em> API based text padding command.
 */
class Complete final : public ICommand
{
public:
  std::string execute(const CommandParams& inputData, const Dependencies&) override;
  constexpr std::string_view description() const noexcept override;
 ~Complete() = default;
};
} //namespace command
} //namespace bot
