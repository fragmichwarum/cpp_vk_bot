# Bot for VK written in C++

## How to start?

You need to fill some fields in `src/long_poll/metadata.hpp`:

```
inline const static int creator_id
  = your id;
```
```
inline const static std::string group_id
  = "";
```
```
inline const static std::string logfile
  = "/path/to/logfile";
```
```
inline const static std::string errfile
  = "/path/to/errfile";
```
```
inline const static std::string access_token
  = "";
```
```
inline const static std::string user_token
  = "";
```

## Build

So, you have multiple build options.
in `cpp_vk_bot` directory:

```
qmake vk.pro
make
```
or, if you don't have/don't want to use Qmake:
```
g++ -c -pipe -Wall -Os -std=gnu++1z -fPIC -I. -Iinclude lib/src/* src/* && g++ -Wl,-O1 -o vk *.o -lcurl -lsqlite3 -lpthread && rm *.o -rf
```
or
```
clang++ -c -pipe -Wall -Os -std=gnu++1z -fPIC -I. -Iinclude lib/src/* src/* && clang++ -Wl,-O1 -o vk *.o -lcurl -lsqlite3 -lpthread && rm *.o -rf
```
