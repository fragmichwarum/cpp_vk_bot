#pragma once

#include "lib/include/document/common_document.hpp"

namespace vk
{
class video : private document::common
{
public:
  vk::attachment::attachments_t search(std::string_view query, std::int64_t count) const;
//  std::shared_ptr<vk::attachment::video_attachment> save_by_link(std::string_view url);
};
}
