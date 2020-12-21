#include "LongPollListener.hpp"

#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wcomment"
#endif
/*!
 * @mainpage Detailed description of https://github.com/oxfffffe/cpp_vk_bot
 *
 *
 * ## How to start?
 *
 * First of all, you have take a look in <b><em>init.json</em></b> file.
 *
 * ```
 * {
 *     "token": {
 *         "access_token": "Access token of your group",
 *         "user_token": "Access token of your user"
 *     },
 *     "group_id": "ID of your group",
 *     "admin_id": Your user ID,
 *     "path": {
 *         "log": "The file where the log will be stored",
 *         "err": "The file where the error log will be stored"
 *     },
 *     "api_v": "5.124"
 * }
 * ```
 *
 * ## Build
 *
 * at <em><b>cpp_vk_bot</b></em> directory:
 *  - <b>mkdir build</b>
 *  - <b>cd build</b>
 *  - <b>cmake ..</b>
 *  - <b>make</b>
 *
 * ## Used libraries
 *  - SQLiteCpp
 *  - simdjson
 *  - libcurl
 */
#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
#  pragma GCC diagnostic pop
#endif

int main()
{
  bot::LongPollListener listener;
  listener.loop();
  return 0;
}
