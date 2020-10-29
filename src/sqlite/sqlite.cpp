#include <stdexcept>
#include "sqlite.hpp"

using std::string;
using std::invalid_argument;
using std::to_string;

int Database::callback(
  void*  not_used,
  int    argc,
  char** argv,
  char** col_name)
{
  return 0;
}

void Database::open() {
  if (rc = sqlite3_open("users.db", &database); rc) {
    throw invalid_argument(sqlite3_errmsg(database));
  }
}

void Database::init_table() {
  string query =
      "CREATE TABLE IF NOT EXISTS USERS ("
      "USER_ID INTEGER NOT NULL UNIQUE, "
      "PREFIX  TEXT NOT NULL);";
  rc =
  sqlite3_exec(
    database,      /* An open database         */
    query.c_str(), /* SQL to be evaluated      */
    callback,      /* Callback function        */
    0,             /* 1st argument to callback */
    &err           /* Error msg written here   */
  );
}

void Database::insert_nickname(const long& user_id, const string& prefix) {
  open();
  string query =
      "REPLACE INTO USERS ('USER_ID', 'PREFIX') "
      "VALUES ('" + to_string(user_id) + "','" + prefix  + "');";
  rc =
  sqlite3_exec(
    database,      /* An open database         */
    query.c_str(), /* SQL to be evaluated      */
    callback,      /* Callback function        */
    0,             /* 1st argument to callback */
    &err           /* Error msg written here   */
  );
}

string Database::return_nickname(const long& user_id) {
  string result;
  sqlite3_stmt* stmt;
  char query[255];

  open();
  snprintf(
    query,
    sizeof query - 1,
    "SELECT PREFIX FROM USERS WHERE USER_ID = %s;",
    to_string(user_id).c_str());
  rc =
  sqlite3_prepare(
    database,       /* An open database         */
    query,          /* SQL to be evaluated      */
    sizeof (query), /* Callback function        */
    &stmt,          /* 1st argument to callback */
    NULL            /* Error msg written here   */
  );
  if (rc == 0) {
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
      result += (const char*)sqlite3_column_text(stmt, 0);
    }
  }
  if (result.empty()) {
    return "";
  } else {
    return result;
  }
}
