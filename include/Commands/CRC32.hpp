#pragma once

#include "../ICommand.hpp"
#include "../lib/include/crc32.hpp"
#include "Utils.hpp"

namespace bot
{
namespace command
{
class CRC32Command final : public ICommand
{
public:
  std::string execute([[maybe_unused]]const CommandParams& inputData) override
  {
    return "0x" + bot::util::longToHexStr(crc32gen(inputData.args.c_str()));
  }
  std::string description() const override
  {
    return "сгенерить CRC-32 хеш-сумму строки";
  }
  std::string trigger() const override
  {
    return "+crc32";
  }
 ~CRC32Command()
  { }
};
} //namespace command
} //namespace bot
