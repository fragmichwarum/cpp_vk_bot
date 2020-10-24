TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -Wall -Wextra -O2
LIBS += -lcurl -lsqlite3

SOURCES += \
        main.cpp \
        src/cmd_handler/cmd_handler.cpp \
        src/cmd_holder/cmds.cpp \
        src/cmd_holder/lib/crc32.cpp \
        src/cmd_holder/lib/sha256.cpp \
        src/logger/logger.cpp \
        src/long_poll/error_handler.cpp \
        src/long_poll/long_poll.cpp \
        src/network/curl.cpp \
        src/sqlite/sqlite.cpp \
        src/splitter/split.cpp \

HEADERS += \
        src/cmd_holder/lib/crc32.hpp \
        src/cmd_holder/lib/json.hpp \
        src/cmd_holder/lib/sha256.hpp \
        src/cmd_holder/cmd_handler.hpp \
        src/cmd_holder/cmd_holder.hpp \
        src/cmd_handler/cmd_handler.hpp \
    	src/long_poll/long_poll.hpp \
        src/long_poll/metadata.hpp \
        src/network/curl.hpp \
        src/sqlite/sqlite.hpp \
        src/splitter/split.hpp \
        src/logger/logger.hpp
