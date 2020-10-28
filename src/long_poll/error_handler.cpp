#include "./long_poll.hpp"

using std::runtime_error;
using std::vector;
using std::string;
using std::tuple;
using std::cerr;
using std::endl;
using std::get;

namespace {
const bool     FATAL = true;
const bool NOT_FATAL = false;
const int    errcode = 0;
const int     errmsg = 1;
const int   errfatal = 2;

const vector<tuple<
/* Error code   */ long,
/* Error message*/ string,
/* Is FATAL?    */ bool>> errors
{
  {  1, "Unknown error",                       NOT_FATAL },
  {  2, "App disabled",                            FATAL },
  {  3, "Unknown method",                          FATAL },
  {  5, "Authorization failed",                    FATAL },
  {  6, "Requestes per second limit exceeded", NOT_FATAL },
  {  7, "Permission denied",                   NOT_FATAL },
  { 10, "Internal server error",               NOT_FATAL },
  { 14, "Captcha needed",                      NOT_FATAL },
  { 15, "Access denied",                       NOT_FATAL },
  { 16, "HTTP authorization failed",               FATAL }
};
} //namespace

void Lp::errors_handle(long error_code) {
  for (auto error : errors) {
    if (get<errcode>(error) == error_code) {
      if (get<errfatal>(error) == FATAL) {
        _logger.write_err(__LINE__, __FILE__, __FUNCTION__, get<errmsg>(error).c_str());
        throw runtime_error(get<errmsg>(error));
      } else {
        _logger.write_err(__LINE__, __FILE__, __FUNCTION__, get<errmsg>(error).c_str());
        get_lp_server();
      }
    }
  }
}
