#include "./long_poll.hpp"

using std::runtime_error;
using std::vector;
using std::string;
using std::tuple;
using std::cerr;
using std::endl;
using std::get;

namespace {
const bool     fatal = true;
const bool non_fatal = false;
const int    errcode = 0;
const int     errmsg = 1;
const int   errfatal = 2;

const vector<tuple<
/* Error code   */ long,
/* Error message*/ string,
/* Is fatal?    */ bool>> errors
{
  {  1, "Unknown error",                       non_fatal },
  {  2, "App disabled",                            fatal },
  {  3, "Unknown method",                          fatal },
  {  5, "Authorization failed",                    fatal },
  {  6, "Requestes per second limit exceeded", non_fatal },
  {  7, "Permission denied",                   non_fatal },
  { 10, "Internal server error",               non_fatal },
  { 14, "Captcha needed",                      non_fatal },
  { 15, "Access denied",                       non_fatal },
  { 16, "HTTP authorization failed",               fatal }
};
} //namespace

void Lp::errors_handle(long error_code) {
  for (auto error : errors) {
    if (get<errcode>(error) == error_code) {
      if (get<errfatal>(error) == fatal) {
        throw runtime_error(get<errmsg>(error));
      } else {
        cerr << get<errmsg>(error) << endl;
        get_lp_server();
      }
    }
  }
}
