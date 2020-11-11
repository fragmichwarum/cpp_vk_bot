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

vector<tuple<long, string, bool>> errors =
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

void Lp::_errors_handle(long errcode) {
  for (auto error : errors) {
    if (not (get<long>(error) == errcode)) {
      continue;
    }
    _logger.write_err(__LINE__, __FILE__, __FUNCTION__, get<string>(error).c_str());

    if (get<bool>(error) == fatal) {
      throw runtime_error(get<string>(error));
    }
    if (get<bool>(error) == non_fatal) {
      _get_lp_server();
    }
  }
}
