#ifndef SQLITE_HPP
#define SQLITE_HPP
#define EMPTY 0

#include <sqlite3.h>
#include <vector>
#include <iostream>

using std::string;
using std::vector;

class Database {
private:
  sqlite3*   database;
  int rc     = 0;
  char* err  = nullptr;
  static int callback(void* not_used, int argc, char* argv[], char** col_name);

public:
  void open();
  void init_table();
  void insert_nickname(const long& user_id, const string& prefix);
  string return_nickname(const long& user_id);
};

#endif //SQLITE_HPP
