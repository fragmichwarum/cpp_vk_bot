TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -lcurl -Wall -Wextra -O2
LIBS += -lcurl -lsqlite3

SOURCES += \
        main.cpp \
        src/cmd_holder/commands/_message_send.cpp \
        src/cmd_holder/commands/os.cpp \
        src/cmd_holder/commands/stat.cpp \
        src/cmd_holder/commands/repeat.cpp \
        src/cmd_holder/commands/lib/crc32.cpp \
        src/cmd_holder/commands/lib/sha256.cpp \
        src/cmd_holder/commands/nickname.cpp \
        src/cmd_holder/commands/_resolve_screen_name.cpp \
        src/cmd_holder/commands/_media_search.cpp \
        src/cmd_holder/commands/crc.cpp \
        src/cmd_holder/commands/sha.cpp \
        src/cmd_holder/commands/vk_media.cpp \
        src/cmd_holder/commands/help.cpp \
        src/cmd_holder/commands/weather.cpp \
        src/cmd_handler/cmd_handler.cpp \
        src/logger/logger.cpp \
        src/long_poll/error_handler.cpp \
        src/long_poll/long_poll.cpp \
        src/network/curl.cpp \
        src/sqlite/sqlite.cpp \
        src/splitter/split.cpp \

HEADERS += \
        src/cmd_holder/commands/lib/crc32.hpp \
        src/cmd_holder/commands/lib/json.hpp \
        src/cmd_holder/commands/lib/sha256.hpp \
        src/cmd_holder/cmd_handler.hpp \
        src/cmd_holder/cmd_holder.hpp \
        src/cmd_handler/cmd_handler.hpp \
    	src/long_poll/long_poll.hpp \
        src/network/curl.hpp \
        src/long_poll/token.hpp \
        src/sqlite/sqlite.hpp \
        src/splitter/split.hpp \
        src/logger/logger.hpp
