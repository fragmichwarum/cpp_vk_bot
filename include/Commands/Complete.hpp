#pragma once

#include "ICommand.hpp"

namespace bot::command
{
/*!
 * @brief <em><b>https://pelevin.gpt.dobro.ai</b></em> API based text padding command.
 */
class Complete final : public ICommand
{
public:
  constexpr std::uint8_t access() const noexcept override;
  constexpr std::string_view description() const noexcept override;
  std::string execute(const CommandParams& inputData, const Dependencies&) override;
 ~Complete() = default;
};
} //namespace bot::command
