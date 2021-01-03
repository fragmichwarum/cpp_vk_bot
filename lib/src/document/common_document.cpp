#include <simdjson.h>

#include "lib/include/lib/network.hpp"
#include "lib/include/document/common_document.hpp"

static void photos_search(vk::attachment::attachments_t& documents, const simdjson::dom::array& items)
{
  for (std::size_t i = 0; i < items.size() && i < 10; i++)
  {
    std::size_t index = rand() % items.size();
    documents.push_back(std::make_shared<vk::attachment::photo_attachment>(
      static_cast<std::int64_t>(items.at(index)["owner_id"]),
      static_cast<std::int64_t>(items.at(index)["id"])
    ));
  }
}

static void video_search(vk::attachment::attachments_t& documents, const simdjson::dom::array& items)
{
  for (std::size_t i = 0; i < items.size() && i < 10; i++)
  {
    std::size_t index = rand() % items.size();
    documents.push_back(std::make_shared<vk::attachment::video_attachment>(
      static_cast<std::int64_t>(items.at(index)["owner_id"]),
      static_cast<std::int64_t>(items.at(index)["id"])
    ));
  }
}

static void docs_search(vk::attachment::attachments_t& documents, const simdjson::dom::array& items)
{
  for (std::size_t i = 0; i < items.size() && i < 10; i++)
  {
    std::size_t index = rand() % items.size();
    documents.push_back(std::make_shared<vk::attachment::document_attachment>(
      static_cast<std::int64_t>(items.at(index)["owner_id"]),
      static_cast<std::int64_t>(items.at(index)["id"]),
      static_cast<std::string_view>(items.at(index)["url"])
    ));
  }
}

vk::attachment::attachments_t vk::document::common::common_search(std::string_view type, std::string_view query, std::int64_t count) const
{
  vk::attachment::attachments_t documents;

  std::string raw_json =
  call(type, user_params({
    { "q",     query.data()  },
    { "count", std::to_string(count)}
  }));

  simdjson::dom::array items = parser->parse(raw_json)["response"]["items"].get_array();

  if (items.size() == 0) return documents;

  if (type == "photos.search") photos_search(documents, items);
  if (type == "video.search")  video_search(documents, items);
  if (type == "docs.search")   docs_search(documents, items);

  return documents;
}
