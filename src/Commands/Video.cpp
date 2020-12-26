#include "JsonUtils.hpp"
#include "Video.hpp"

constexpr std::uint8_t bot::command::Video::access() const noexcept
{
  return static_cast<std::uint8_t>(bot::Access::user);
}

constexpr std::string_view bot::command::Video::description() const noexcept
{
  return "поиск видеозаписей ВК";
}

std::string bot::command::Video::execute(const CommandParams& params, const Dependencies& /* unused */)
{
  if (params.args.empty()) return "Задана пустая строка.";

  return jsonUtils::sendRandomMedia("video.search", params.args, params.peer_id);
}
