#pragma once

#include <string>
#include <memory>
#include <list>

namespace vk::attachment
{
class base_attachment
{
public:
  explicit
  base_attachment (std::int32_t id_, std::int32_t owner_id_)
    : id(id_), owner_id(owner_id_)
  { }
  virtual ~base_attachment() = default;
  virtual std::string value() const noexcept = 0;
  virtual std::string type()  const noexcept = 0;
  virtual bool empty()        const noexcept { return true; }

protected:
  std::int32_t id;
  std::int32_t owner_id;
};

class photo_attachment : public base_attachment
{
public:
  explicit
  photo_attachment(std::int32_t id_, std::int32_t owner_id_)
    : base_attachment (id_, owner_id_)
  { }
  std::string value() const noexcept override { return "photo" + std::to_string(id) + '_' + std::to_string(owner_id); }
  std::string type()  const noexcept override { return "photo"; }
  bool empty()        const noexcept override { return false; }
};

class video_attachment : public base_attachment
{
public:
  explicit
  video_attachment(std::int32_t id_, std::int32_t owner_id_)
    : base_attachment (id_, owner_id_)
  { }
  std::string value() const noexcept override { return "video" + std::to_string(id) + '_' + std::to_string(owner_id); }
  std::string type()  const noexcept override { return "video"; }
  bool empty()        const noexcept override { return false; }
};

class audio_attachment : public base_attachment
{
public:
  explicit
  audio_attachment(std::int32_t id_, std::int32_t owner_id_)
    : base_attachment (id_, owner_id_)
  { }
  std::string value() const noexcept override { return "audio" + std::to_string(id) + '_' + std::to_string(owner_id); }
  std::string type()  const noexcept override { return "audio"; }
  bool empty()        const noexcept override { return false; }
};

class document_attachment : public base_attachment
{
public:
  explicit
  document_attachment(std::int32_t id_, std::int32_t owner_id_, std::string_view url)
    : base_attachment (id_, owner_id_), _raw_url(url.data())
  { }
  std::string value()   const noexcept override { return "doc" + std::to_string(id) + '_' + std::to_string(owner_id); }
  std::string type()    const noexcept override { return "doc"; }
  bool empty()          const noexcept override { return false; }
  std::string raw_url() const noexcept { return _raw_url; }

private:
  const char* _raw_url;
};

class audio_message_attachment : public base_attachment
{
public:
  explicit
  audio_message_attachment(std::int32_t id_, std::int32_t owner_id_, std::string_view raw_ogg, std::string_view raw_mp3)
    : base_attachment (id_, owner_id_), _raw_ogg(raw_ogg.data()), _raw_mp3(raw_mp3.data())
  { }
  std::string value()   const noexcept override { return "audio_message" + std::to_string(id) + '_' + std::to_string(owner_id); }
  std::string type()    const noexcept override { return "audio_message"; }
  bool empty()          const noexcept override { return false; }
  std::string raw_ogg() const noexcept { return _raw_ogg; }
  std::string raw_mp3() const noexcept { return _raw_mp3; }

private:
  const char* _raw_ogg;
  const char* _raw_mp3;
};

class wall_attachment : public base_attachment
{
public:
  explicit
  wall_attachment(std::int32_t id_, std::int32_t from_id_)
    : base_attachment(id_, from_id_)
  { }
  std::string value()   const noexcept override { return "wall" + std::to_string(id) + '_' + std::to_string(owner_id); }
  std::string type()    const noexcept override { return "wall"; }
  bool empty()          const noexcept override { return false; }
};

std::shared_ptr<photo_attachment> photo_cast(const std::shared_ptr<base_attachment>& pointer) noexcept;
std::shared_ptr<audio_attachment> audio_cast(const std::shared_ptr<base_attachment>& pointer) noexcept;
std::shared_ptr<video_attachment> video_cast(const std::shared_ptr<base_attachment>& pointer) noexcept;
std::shared_ptr<document_attachment> document_cast(const std::shared_ptr<base_attachment>& pointer) noexcept;
std::shared_ptr<audio_message_attachment> audio_message_cast(const std::shared_ptr<base_attachment>& pointer) noexcept;
std::shared_ptr<wall_attachment> wall_cast(const std::shared_ptr<wall_attachment>& pointer) noexcept;

using attachments_t = std::list<std::shared_ptr<attachment::base_attachment>>;
const inline attachments_t undefined_attachment = { std::make_shared<vk::attachment::photo_attachment>(-1, -1) };
} // namespace vk::attachment
