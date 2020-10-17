#ifndef cmd_holder_HPP
#define cmd_holder_HPP
#define USE_NICKNAME true
#define NOT_USE_NICKNAME false

#include <fstream> //stat command
#include "../network/curl.hpp"
#include "../sqlite/sqlite.hpp"
#include "../splitter/split.hpp"

using std::to_string;
using std::string;

class cmd_holder {
private:
  void          _empty_query();
  inline string _attachment_type(const string& method);
  inline void   _media_not_found(const string& type);
  void          _message_send(string text, bool use_nickname);
  string        _resolve_screen_name(const string& screen_name);
  void          _media_search(const vector<string>& tokenized_message,
                              const string& method);
  Database      _database;
  const string  _message;
  const long    _peer_id;
  const long    _from_id;
  const string  _nickname = _database.return_nickname(_from_id);
  const vector<string> _splitted_message = split(_message);

public:
  cmd_holder()                        = delete;
  cmd_holder& operator=(cmd_holder&)  = delete;
  cmd_holder& operator=(cmd_holder&&) = delete;

  cmd_holder(const string& message, const long& peer_id, const long& from_id)
    : _message(message)
    , _peer_id(peer_id)
    , _from_id(from_id)
  { }

  void crc32_command();

  void sha256_command();

  void picture_command();

  void video_command();

  void document_command();

  void weather_command();

  void help_command();

  void add_nickname_command();

  void remove_nickname_command();

  void nickname_command();

  void repeat_command();

  void stat_command();

  void os_command();
};

#endif //cmd_holder_HPP
