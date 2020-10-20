# Bot for VK written in C++

## How to start?

You need to fill some fields in `src/long_poll/token.hpp`:

```
const static std::string access_token
    = "your group token";
```

```
const static std::string user_token
    = "your user token";
```

```
const static std::string group_id
    = "id of your group";
```

```
const static int admin_id
    = your user id;
```

```
static std::string api_version
    = "5.80";
```

Next please do in `cpp_vk_bot` directory:

```
make
```
