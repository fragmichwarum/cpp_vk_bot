#include <simdjson.h>

#include "Network.hpp"
#include "Utility.hpp"
#include "VkAPI.hpp"
#include "JsonUtils.hpp"

static bot::Network net;
static bot::VkAPI api;

static inline std::string getAttachmentType(std::string_view method) noexcept
{
  if (method == "photos.search") return "photo";
  if (method == "video.search") return "video";
  if (method == "docs.search") return "doc";
  return "";
}

static inline std::string attachmentList(std::string_view method, const simdjson::dom::array& items)
{
  std::string docs;
  for (uint8_t i = 0; i < items.size() && i < 10; i++) {
    long index = rand() % items.size();
    docs +=
      getAttachmentType(method) +
      std::to_string(items.at(index)["owner_id"].get_int64()) + '_' +
      std::to_string(items.at(index)["id"].get_int64()) + ',';
  }

  return docs;
}

static inline std::string getMedia(std::string_view method, std::string_view keyword)
{
  std::string response = api.mediaSearch(method, keyword, /* count = */ 25);

  static
  simdjson::dom::parser parser;
  simdjson::dom::array items = parser.parse(response)["response"]["items"].get_array();

  if (items.size() == 0) return "";

  return attachmentList(method, items);
}

std::string bot::jsonUtils::sendRandomMedia(std::string_view type, std::string_view keyword, long peer_id)
{
  std::string attachments = getMedia(type, keyword);
  if (attachments.empty()) return "Не найдено документов.";

  api.sendMessage("", peer_id, {{"attachment", attachments}});
  return "";
}

std::string bot::jsonUtils::pickRandomChatUser(long peer_id)
{
  std::string response = api.getConversationMembers(peer_id);

  static
  simdjson::dom::parser parser;
  simdjson::dom::object parsed = parser.parse(response);

  if (parsed.begin().key() == "error" && parsed["error"]["error_code"].get_int64() == 917L)
  {
    return "Упс, кажется у бота нет админки.";
  }

  std::size_t size = parsed["response"]["profiles"].get_array().size();
  if (size == 0) return "Что-то пошло не так.";

  simdjson::dom::object person = parsed["response"]["profiles"].get_array().at(rand() % size);

  std::string id        (std::to_string(person["id"].get_int64()));
  std::string firstName (person["first_name"].get_c_str());
  std::string lastName  (person["last_name"].get_c_str());

  return "Хмм, я думаю, что @id" + id + '('  + firstName + ' ' + lastName + ')';
}

static inline std::string onlineUsersList(const simdjson::dom::object& response)
{
  std::string people = "Список людей онлайн:\n";

  for (const simdjson::dom::element& profile : response["response"]["profiles"].get_array())
  {
    if (profile["online"].get_int64() == 1)
    {
      std::string id        (std::to_string(profile["id"].get_int64()));
      std::string firstName (profile["first_name"].get_c_str());
      std::string lastName  (profile["last_name"].get_c_str());
      people += "@id" + id + '(' + firstName + ' ' + lastName + ")\n";
    }
  }
  return people;
}

std::string bot::jsonUtils::getOnlineUsers(long peer_id)
{
  static
  simdjson::dom::parser parser;
  simdjson::dom::object response = parser.parse(api.getConversationMembers(peer_id));

  if (response.begin().key() == "error" && response["error"]["error_code"].get_int64() == 917L)
  {
    return "Упс, кажется у бота нет админки.";
  }

  return onlineUsersList(response);
}

std::string bot::jsonUtils::completeText(std::string_view text)
{
  std::string response =
    net.requestdata("https://pelevin.gpt.dobro.ai/generate/",
    util::toJson({{"prompt", text.data()}, {"length", "50"}}));

  static
  simdjson::dom::parser parser;
  simdjson::dom::object AIObject = parser.parse(response);

  if (AIObject.begin().key() != "replies") return "Ошибка генерации текста.";

  return text.data() + std::string{AIObject["replies"].at(0).get_c_str()};
}

std::string bot::jsonUtils::uploadCatImage(long peer_id)
{
  static
  simdjson::dom::parser parser;
  simdjson::dom::array catAPIArray = parser.parse(net.request("https://api.thecatapi.com/v1/images/search", {}));

  if (catAPIArray.at(0)["url"].is_null()) return "Что-то пошло не по плану.";

  return api.processAttachmentUploading("photo", "cat.jpg", static_cast<const char*>(catAPIArray.at(0)["url"]), peer_id);
}

static inline std::string currencyList(const simdjson::dom::object& response)
{
  static constexpr const char* codes[] = { "GBP", "BYN", "USD", "EUR", "KZT", "PLN", "UAH", "JPY" };

  std::string list = "Курс валют:\n";

  for (std::string_view currency : codes)
  {
    list += std::to_string(response["Valute"][currency]["Nominal"].get_int64());
    list += ' ';
    list += response["Valute"][currency]["Name"].get_c_str();
    list += " -> ";
    list += std::to_string(response["Valute"][currency]["Value"].get_double());
    list += "₽\n";
  }

  return list;
}

std::string bot::jsonUtils::getCurrencyList()
{
  static
  simdjson::dom::parser parser;

  return currencyList(parser.parse(net.request("https://www.cbr-xml-daily.ru/daily_json.js", {})));
}
