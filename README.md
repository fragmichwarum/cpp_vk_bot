# Bot for VK written in C++

## How to start?

You need to fill some fields in `src/long_poll/metadata.hpp`:

```
...std::string yandex_key
    = "your Yandex API key";
```

```
...std::string access_token
    = "your group token";
```

```
...std::string user_token
    = "your user token";
```

```
...std::string group_id
    = "id of your group";
```

```
...int admin_id
    = your user id;
```

```
...std::string api_version
    = "5.80";
```

```
...std::string logfile
    = "/path/to/logfile";
```

```
...std::string errfile
    = "/path/to/errfile";
```

Next please do in `cpp_vk_bot` directory:

```
make
```
