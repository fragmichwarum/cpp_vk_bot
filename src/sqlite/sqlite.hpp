#pragma once

#include <sqlite3.h>
#include <string>
#include <vector>

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

  void insert_role(
    const long&        user_id,
    const long&        peer_id,
    const std::string& role
  );

  std::vector<uint32_t> get_roles(
    const long&        peer_id,
    const std::string& role
  );
};
