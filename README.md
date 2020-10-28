# Bot for VK written in C++

## How to start?

You need to fill some fields in `src/long_poll/metadata.hpp`:

```
inline const static int admin_id
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
inline const static std::string yandex_key
  = "";
```
```
inline const static std::string access_token
  = "";
```
```
inline const static std::string user_token
  = "";
```
Next please do in `cpp_vk_bot` directory:

```
make
```
