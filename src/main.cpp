#include "long_poll.hpp"

int main(void) {
  bot::Long_poll_handler lp;
  lp.loop();
}


/*
  Compiled program size with different options.
 |----------------------------------------------------|----------|
 | g++      -c -pipe -Wall -Os -std=gnu++1z -fPIC     |     220K |
 | g++      -c -pipe -Wall -O1 -std=gnu++1z -fPIC     |     248K |
 | g++      -c -pipe -Wall -O2 -std=gnu++1z -fPIC     |     284K |
 | clang++  -c -pipe -Wall -Os -std=gnu++1z -fPIC     |     288K |
 | clang++  -c -pipe -Wall -O2 -std=gnu++1z -fPIC     |     324K |
 | g++      -c -pipe -Wall -O3 -std=gnu++1z -fPIC     |     328K |
 | clang++  -c -pipe -Wall -O3 -std=gnu++1z -fPIC     |     332K |
 | clang++  -c -pipe -Wall -O1 -std=gnu++1z -fPIC     |     632K |
 | clang++  -c -pipe -Wall     -std=gnu++1z -fPIC     |     744K |
 | g++      -c -pipe -Wall     -std=gnu++1z -fPIC     |     760K |
 |----------------------------------------------------|----------|
*/
