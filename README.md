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
mkdir build
cd build
cmake ..
make -j{1..128}
