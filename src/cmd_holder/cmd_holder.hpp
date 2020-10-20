#ifndef cmd_holder_HPP
#define cmd_holder_HPP


#include <fstream> //stat cmd
#include "../network/curl.hpp"
#include "../sqlite/sqlite.hpp"
#include "../splitter/split.hpp"

using std::to_string;
using std::string;
const bool     USE_NICKNAME = true;
const bool NOT_USE_NICKNAME = false;

class cmd_holder {
private:
  void          _empty_query();
  inline string _attachment_type(const string& method);
  inline void   _media_not_found(const string& type);
  void          _message_send(string text, bool use_nickname);
  void          _media_search(const vector<string>& tokenized_message, const string& method);
  Database      _database;
  const string  _message;
  const long    _peer_id;
  const long    _from_id;
  const string  _nickname = _database.return_nickname(_from_id);
  const vector<string> _splitted_message = split(_message);

public:
  cmd_holder(const string& message, const long& peer_id, const long& from_id)
    : _message(message)
    , _peer_id(peer_id)
    , _from_id(from_id)
  { }

  static void append_vkparams(map<string, string>& map);

  static string append_vkurl(const string& method);

  void crc32_cmd();

  void sha256_cmd();

  void picture_cmd();

  void video_cmd();

  void document_cmd();

  void weather_cmd();

  void help_cmd();

  void add_nickname_cmd();

  void remove_nickname_cmd();

  void nickname_cmd();

  void repeat_cmd();

  void stat_cmd();

  void os_cmd();
};

#endif //cmd_holder_HPP
