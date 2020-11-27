#pragma once

#include "ICommand.hpp"
#include "Utils.hpp"
#include "../lib/include/crc32.hpp"

namespace bot
{
namespace command
{
class CRC32Command final : public ICommand
{
public:
  const std::string execute([[maybe_unused]]const CommandParams& inputData) override
  {
    return not inputData.args.empty() ? util::emptyArgs() : "0x" + util::longToHexStr(crc32gen(inputData.args.c_str()));
  }
  const std::string description() const override
  {
    return "сгенерить CRC-32 хеш-сумму строки";
  }
  const std::string trigger() const override
  {
    return "+crc32";
  }
 ~CRC32Command()
  { }
};
} //namespace command
} //namespace bot
