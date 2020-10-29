#pragma once

#include <sqlite3.h>
#include <string>

class Database {
private:
  sqlite3* database;
  int      rc        = 0;
  char*    err       = nullptr;
  bool     is_opened = false;
  static int callback(
    void* not_used,
    int argc,
    char* argv[],
    char** col_name
  );

public:
  void open();
  void init_table();
  void insert(const long& user_id, const std::string& prefix);
  std::string get(const long& user_id);
};
