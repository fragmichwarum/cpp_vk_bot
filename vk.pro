TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -Wall -Wextra -Os
LIBS += -lcurl -lsqlite3 -lpthread

INCLUDEPATH += \
        include/

SOURCES += \
        src/cmd_handler.cpp       \
        src/cmds.cpp              \
        src/curl.cpp              \
        src/database.cpp          \
        src/logger.cpp            \
        src/long_poll.cpp         \
        src/main.cpp              \
        lib/src/crc32.cpp         \
        lib/src/wchar.cpp         \
        src/utils.cpp             \
        src/vk_api.cpp

HEADERS += \
        include/cmd_handler.hpp   \
        include/curl.hpp          \
        include/database.hpp      \
        include/error_handler.hpp \
        include/logger.hpp        \
        include/long_poll.hpp     \
        include/metadata.hpp      \
        include/utils.hpp         \
        include/vk_api.hpp        \
        lib/include/crc32.hpp     \
        lib/include/json.hpp      \
        lib/include/wchar.hpp     \
