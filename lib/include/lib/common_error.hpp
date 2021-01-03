#pragma once

#include <stdexcept>

namespace vk
{
class common_error : public std::exception
{
public:
  common_error()                                = delete;
  common_error(const common_error&)             = delete;
  common_error(common_error&&)                  = delete;
  common_error& operator=(const common_error&)  = delete;
  common_error& operator=(common_error&&)       = delete;

  common_error(const char* file_, std::int32_t line_, std::int32_t id_, const char* message_)
    : file    (file_)
    , msg     (message_)
    , line    (std::to_string(line_))
    , id      (std::to_string(id_))
  { }

  const char* what() const noexcept override
  {
    static std::string what = file + ':' + line + " [vk.common_error." + id + "]: " + msg;
    return what.c_str();
  }

private:
  std::string file;
  std::string msg;
  std::string line;
  std::string id;
};
}
