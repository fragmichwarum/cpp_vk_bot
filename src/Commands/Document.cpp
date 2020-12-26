#include "JsonUtils.hpp"
#include "Document.hpp"

constexpr std::uint8_t bot::command::Document::access() const noexcept
{
  return static_cast<std::uint8_t>(bot::Access::user);
}

constexpr std::string_view bot::command::Document::description() const noexcept
{
  return "поиск документов ВК";
}

std::string bot::command::Document::execute(const CommandParams& params, const Dependencies& deps)
{
  if (params.args.empty()) return "Задана пустая строка.";

  return deps.jsonUtils->sendRandomMedia("docs.search", params.args, params.peer_id);
}
