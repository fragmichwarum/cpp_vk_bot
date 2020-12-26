#include "JsonUtils.hpp"
#include "Picture.hpp"

constexpr std::uint8_t bot::command::Picture::access() const noexcept
{
  return static_cast<std::uint8_t>(bot::Access::user);
}

constexpr std::string_view bot::command::Picture::description() const noexcept
{
  return "поиск картинок в ВК";
}

std::string bot::command::Picture::execute(const CommandParams& params, const Dependencies& /* unused */)
{
  if (params.args.empty()) return "Задана пустая строка.";

  return jsonUtils::sendRandomMedia("photos.search", params.args, params.peer_id);
}
