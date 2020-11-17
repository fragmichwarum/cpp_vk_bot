# Bot for VK written in C++

## How to start?

You need to fill some fields in `init.json`:
```
{
	"tokens": {
		"access_token": "Access token of your group",
		"user_token": "Access token of your user"
	},
	"group_id": "ID of your group",
	"admin_id": Your user ID,
	"path": {
		"log": "The file where the log will be stored",
		"err": "The file where the error log will be stored"
	},
	"api_v": "5.124"
}
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
