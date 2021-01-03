#pragma once

#include "lib/include/document/common_document.hpp"

namespace vk
{
class audio : private base_object
{
public:
  std::string get_upload_server() const;
  void save(std::string_view artist, std::string_view title, std::string_view filename, std::string_view raw_server);
};
}
