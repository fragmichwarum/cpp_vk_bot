#include "lib/include/attachment/attachment.hpp"

std::shared_ptr<vk::attachment::photo_attachment> vk::attachment::photo_cast(const std::shared_ptr<vk::attachment::base_attachment>& pointer) noexcept
{
  return std::static_pointer_cast<photo_attachment>(pointer);
}

std::shared_ptr<vk::attachment::audio_attachment> vk::attachment::audio_cast(const std::shared_ptr<vk::attachment::base_attachment>& pointer) noexcept
{
  return std::static_pointer_cast<audio_attachment>(pointer);
}

std::shared_ptr<vk::attachment::video_attachment> vk::attachment::video_cast(const std::shared_ptr<vk::attachment::base_attachment>& pointer) noexcept
{
  return std::static_pointer_cast<video_attachment>(pointer);
}

std::shared_ptr<vk::attachment::document_attachment> vk::attachment::document_cast(const std::shared_ptr<vk::attachment::base_attachment>& pointer) noexcept
{
  return std::static_pointer_cast<document_attachment>(pointer);
}

std::shared_ptr<vk::attachment::audio_message_attachment> vk::attachment::audio_message_cast(const std::shared_ptr<vk::attachment::base_attachment>& pointer) noexcept
{
  return std::static_pointer_cast<audio_message_attachment>(pointer);
}

std::shared_ptr<vk::attachment::wall_attachment> vk::attachment::wall_cast(const std::shared_ptr<vk::attachment::wall_attachment>& pointer) noexcept
{
  return std::static_pointer_cast<wall_attachment>(pointer);
}
