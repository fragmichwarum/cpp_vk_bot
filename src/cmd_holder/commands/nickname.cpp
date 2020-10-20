#include "../cmd_holder.hpp"

void cmd_holder::add_nickname_cmd() {
  _database.insert_nickname(_from_id, _splitted_message[1]);
  _message_send("Никнейм успешно установлен\n", NOT_USE_NICKNAME);
}

void cmd_holder::remove_nickname_cmd() {
  _database.insert_nickname(_from_id, "");
  if (_nickname != "") {
    _message_send("Никнейм успешно удалён", NOT_USE_NICKNAME);
  } else {
    _message_send("Никнейм не установлен", NOT_USE_NICKNAME);
  }
}

void cmd_holder::nickname_cmd() {
  if (_message == "+никнейм") {
    _empty_query();
  } else if (_splitted_message[1] == "удалить") {
    remove_nickname_cmd();
  } else {
    add_nickname_cmd();
  }
}
