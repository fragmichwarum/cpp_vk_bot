#pragma once

#define EMPTY 0

#include <sqlite3.h>
#include <string>
#include <stdexcept>

class Database {
private:
  sqlite3*   database;
  int rc     = 0;
  char* err  = nullptr;
  static int callback(void* not_used, int argc, char* argv[], char** col_name);

public:
  void open();
  void init_table();
  void insert_nickname(const long& user_id, const std::string& prefix);
  std::string return_nickname(const long& user_id);
};
