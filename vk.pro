TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -lcurl
LIBS += -lcurl

SOURCES += \
        command_handler.cpp \
        curl.cpp \
        help_autogen.cpp \
        init_bot.cpp \
        main.cpp \
        /usr/local/include/src/Lightning/Hash/crc32.cpp \
        /usr/local/include/src/Lightning/Hash/sha256.cpp \
        tokenizer.cpp

HEADERS += \
    command_handler.hpp \
        curl.hpp \
    help_autogen.hpp \
    init_bot.hpp \
    token.hpp \
        tokenizer.hpp \

