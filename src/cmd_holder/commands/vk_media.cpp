#include "../cmd_holder.hpp"

void cmd_holder::document_cmd() {
  if (_message == "+документ" or
      _message == "+доки")
  {
    _empty_query();
  } else {
    _media_search(_splitted_message, "docs.search");
  }
}

void cmd_holder::picture_cmd() {
  if (_message == "+пикча" or
      _message == "+фото")
  {
    _empty_query();
  } else {
    _media_search(_splitted_message, "photos.search");
  }
}

void cmd_holder::video_cmd() {
  if (_message == "+видео" or
      _message == "+видос")
  {
    _empty_query();
  } else {
    _media_search(_splitted_message, "video.search");
  }
}
