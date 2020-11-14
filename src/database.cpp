#include <stdexcept>

#include "database.hpp"

using std::string;
using std::invalid_argument;
using std::to_string;
using bot::Database;

int Database::callback(
  [[maybe_unused]] void*  not_used,
  [[maybe_unused]] int    argc,
  [[maybe_unused]] char** argv,
  [[maybe_unused]] char** col_name)
{
  return 0;
}

void Database::open() {
  if (rc = sqlite3_open("users.db", &database); rc) {
    throw invalid_argument(sqlite3_errmsg(database));
  }
  is_opened = true;
}

void Database::init_table() {
  string query =
      "CREATE TABLE IF NOT EXISTS USERS ("
      "USER_ID INTEGER NOT NULL, "
      "PEER_ID INTEGER NOT NULL, "
      "ROLE TEXT NOT NULL);";
  sqlite3_exec(
    database,      /* An open database         */
    query.c_str(), /* SQL to be evaluated      */
    callback,      /* Callback function        */
    0,             /* 1st argument to callback */
    &err           /* Error msg written here   */
  );
}

/*
  Временный костыль по причине того,
  что я не умею пользоваться СУБД.
*/
void Database::insert_role(const long& user_id, const long& peer_id, const std::string& role) {
  if (not is_opened) {
    open();
  }
  string delete_query =
    "DELETE FROM USERS WHERE USER_ID = " + to_string(user_id) + " AND PEER_ID = " + to_string(peer_id) + ";";

  sqlite3_exec(
    database,             /* An open database         */
    delete_query.c_str(), /* SQL to be evaluated      */
    callback,             /* Callback function        */
    0,                    /* 1st argument to callback */
    &err                  /* Error msg written here   */
  );

  string role_query =
    "INSERT INTO USERS('USER_ID','PEER_ID','ROLE') "
    "SELECT USER_ID,PEER_ID,ROLE "
    "FROM (SELECT '" + to_string(user_id) + "' AS USER_ID, "
    "'" + to_string(peer_id) + "' AS PEER_ID, "
    "'" + role + "' AS ROLE) T "
    "WHERE NOT EXISTS (SELECT 1 FROM USERS WHERE USERS.USER_ID = T.USER_ID AND USERS.PEER_ID = T.PEER_ID);";

  sqlite3_exec(
    database,           /* An open database         */
    role_query.c_str(), /* SQL to be evaluated      */
    callback,           /* Callback function        */
    0,                  /* 1st argument to callback */
    &err                /* Error msg written here   */
  );
}


std::vector<long> Database::get_by_role(const long& peer_id, const std::string& role) {
  string result;
  sqlite3_stmt* stmt;
  std::vector<long> moderators;
  std::string query = "SELECT USER_ID FROM USERS WHERE ROLE = '" + role + "' AND PEER_ID = '" + to_string(peer_id) + "';";
  if (not is_opened) {
    open();
  }
  rc =
  sqlite3_prepare(
    database,       /* An open database         */
    query.c_str(),  /* SQL to be evaluated      */
    query.size(),   /* Callback function        */
    &stmt,          /* 1st argument to callback */
    NULL            /* Error msg written here   */
  );
  if (not rc) {
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
      moderators.push_back(std::stoul(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))));
    }
    sqlite3_finalize(stmt);
  }
  return moderators;
}

std::vector<long> Database::get_peer_ids() {
  string result;
  sqlite3_stmt* stmt;
  std::vector<long> moderators;
  std::string query = "SELECT PEER_ID FROM USERS GROUP BY PEER_ID";
  if (not is_opened) {
    open();
  }
  rc =
  sqlite3_prepare(
    database,       /* An open database         */
    query.c_str(),  /* SQL to be evaluated      */
    query.size(),   /* Callback function        */
    &stmt,          /* 1st argument to callback */
    NULL            /* Error msg written here   */
  );
  if (not rc) {
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
      moderators.push_back(std::stoul(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))));
    }
    sqlite3_finalize(stmt);
  }
  return moderators;
}
