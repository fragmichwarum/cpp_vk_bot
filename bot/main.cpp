#include "bot/include/long_poll_api/long_poll_api.hpp"


int main()
{
  bot::long_poll_handler lp_handler;

  lp_handler.loop();

  return 0;
}
