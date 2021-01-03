#include <simdjson.h>

#include "lib/include/events/message_new.hpp"

vk::event::message_new::message_new(std::int64_t peer_id, std::int64_t from_id, std::string_view text, std::string raw_json, const simdjson::dom::array& attachments)
{
  this->_peer_id  = peer_id;
  this->_from_id  = from_id;
  this->_text     = text;
  this->_raw_json = raw_json;

  get_attachments(attachments);
}

vk::event::message_new::message_new(std::string_view raw_json)
{

  static simdjson::dom::parser parser;
  simdjson::dom::object message = parser.parse(raw_json)["object"]["message"];


  _raw_json  = raw_json;
  _from_id   = static_cast<std::int64_t>(message["from_id"]);
  _peer_id   = static_cast<std::int64_t>(message["peer_id"]);
  _text      = static_cast<std::string_view>(message["text"]);

  if (message["reply_message"].is_object())
  {
    get_reply(static_cast<simdjson::dom::object>(message["reply_message"]));
  }
  if (message["attachments"].is_array())
  {
    get_attachments(static_cast<simdjson::dom::array>(message["attachments"]));
  }
  if (static_cast<simdjson::dom::array>(message["fwd_messages"]).size() != 0)
  {
    get_fwd_messages(static_cast<simdjson::dom::array>(message["fwd_messages"]));
  }
}

void vk::event::message_new::get_fwd_messages(const simdjson::dom::array& messages)
{
  for (const simdjson::dom::element& fwd_message : messages)
  {
    _fwd_messages.push_back(
      std::make_shared<message_new>(
        static_cast<std::int64_t>(fwd_message["peer_id"]),
        static_cast<std::int64_t>(fwd_message["from_id"]),
        static_cast<std::string_view>(fwd_message["text"]),
        simdjson::to_string(fwd_message),
        static_cast<simdjson::dom::array>(fwd_message["attachments"])
      )
    );
  }
  _has_fwd_messages = true;
}

void vk::event::message_new::get_reply(const simdjson::dom::object& object)
{
  _reply = std::make_shared<message_new>(
    static_cast<std::int64_t>(object["peer_id"]),
    static_cast<std::int64_t>(object["from_id"]),
    static_cast<std::string_view>(object["text"]),
    simdjson::to_string(object),
    object["attachments"].get_array()
  );
  _has_reply = true;
}

static std::shared_ptr<vk::attachment::photo_attachment> get_photo(const simdjson::dom::object& attachment)
{
  return
  std::make_shared<vk::attachment::photo_attachment>(
    static_cast<std::int64_t>(attachment["photo"]["id"]),
    static_cast<std::int64_t>(attachment["photo"]["owner_id"])
  );
}

static std::shared_ptr<vk::attachment::video_attachment> get_video(const simdjson::dom::object& attachment)
{
  return
  std::make_shared<vk::attachment::video_attachment>(
    static_cast<std::int64_t>(attachment["video"]["id"]),
    static_cast<std::int64_t>(attachment["video"]["owner_id"])
  );
}

static std::shared_ptr<vk::attachment::document_attachment> get_doc(const simdjson::dom::object& attachment)
{
  return
  std::make_shared<vk::attachment::document_attachment>(
    static_cast<std::int64_t>(attachment["doc"]["id"]),
    static_cast<std::int64_t>(attachment["doc"]["owner_id"]),
    static_cast<std::string_view>(attachment["doc"]["url"])
  );
}

static std::shared_ptr<vk::attachment::audio_attachment> get_audio(const simdjson::dom::object& attachment)
{
  return
  std::make_shared<vk::attachment::audio_attachment>(
    static_cast<std::int64_t>(attachment["audio"]["id"]),
    static_cast<std::int64_t>(attachment["audio"]["owner_id"])
  );
}

static std::shared_ptr<vk::attachment::audio_message_attachment> get_audio_message(const simdjson::dom::object& attachment)
{
  return
  std::make_shared<vk::attachment::audio_message_attachment>(
    static_cast<std::int64_t>(attachment["audio_message"]["id"]),
    static_cast<std::int64_t>(attachment["audio_message"]["owner_id"]),
    static_cast<std::string_view>(attachment["audio_message"]["link_ogg"]),
    static_cast<std::string_view>(attachment["audio_message"]["link_mp3"]));
}

static std::shared_ptr<vk::attachment::wall_attachment> get_wall(const simdjson::dom::object& attachment)
{
  return
  std::make_shared<vk::attachment::wall_attachment>(
    static_cast<std::int64_t>(attachment["wall"]["from_id"]),
    static_cast<std::int64_t>(attachment["wall"]["id"])
  );
}

void vk::event::message_new::get_attachments(const simdjson::dom::array& attachments)
{
  for (const simdjson::dom::object& attachment : attachments)
  {
    std::string type = static_cast<std::string>(attachment["type"]);
    if (type == "photo")         _attachments.push_back(get_photo(attachment));
    if (type == "video")         _attachments.push_back(get_video(attachment));
    if (type == "doc")           _attachments.push_back(get_doc(attachment));
    if (type == "audio")         _attachments.push_back(get_audio(attachment));
    if (type == "wall")          _attachments.push_back(get_wall(attachment));
    if (type == "audio_message") _attachments.push_back(get_audio_message(attachment));
  }
}

vk::attachment::attachments_t vk::event::message_new::attachments() const noexcept
{
  return _attachments;
}

std::string vk::event::message_new::text() const noexcept
{
  return _text;
}

std::int64_t vk::event::message_new::from_id() const noexcept
{
  return _from_id;
}

std::int64_t vk::event::message_new::peer_id() const noexcept
{
  return _peer_id;
}

bool vk::event::message_new::has_reply() const noexcept
{
  return _has_reply;
}

bool vk::event::message_new::has_fwd_messages() const noexcept
{
  return _has_fwd_messages;
}

std::string vk::event::message_new::dump() const noexcept
{
  return _raw_json;
}

std::list<std::shared_ptr<vk::event::message_new>> vk::event::message_new::fwd_messages() const
{
  if (_has_fwd_messages) return _fwd_messages;
  else throw std::runtime_error("Attempting accessing empty forward message list");
}

vk::event::message_new vk::event::message_new::reply() const
{
  if (_has_reply) return *_reply;
  else throw std::runtime_error("Attempting accessing empty reply");
}
