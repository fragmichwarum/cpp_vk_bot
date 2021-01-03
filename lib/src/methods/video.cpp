#include <simdjson.h>

#include "lib/include/lib/network.hpp"
#include "lib/include/methods/video.hpp"

vk::attachment::attachments_t vk::video::search(std::string_view query, std::int64_t count) const
{
  return common_search("video.search", query, count);
}

//std::shared_ptr<vk::attachment::video_attachment> vk::video::save_by_link(std::string_view url)
//{
//  std::string response =
//  call("video.save", user_params({
//    { "link", url.data()  }
//  }));

//  printf("%s\n", response.c_str());

//  simdjson::dom::object object = parser->parse(response)["response"];

//  printf("%s\n", network->request(static_cast<std::string_view>(object["upload_url"]), {}).data());

//  return {};
////  return
////  std::make_unique<vk::attachment::video_attachment>(
////    static_cast<long>(object["video_id"]),
////    static_cast<long>(object["owner_id"])
////  );
//}
