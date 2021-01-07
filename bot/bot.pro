TEMPLATE	= app
CONFIG		= console c++17
CONFIG		-= bundle
CONFIG		-= qt

QMAKE_CXXFLAGS	+= \
            -Wall -Wextra -Wpedantic -Wsign-promo   \
            -Wcast-align -Wcast-qual -Wconversion   \
            -Wctor-dtor-privacy -Wenum-compare      \
            -Wfloat-equal -Wnon-virtual-dtor        \
            -Wold-style-cast -Woverloaded-virtual   \
            -Wredundant-decls -Wsign-conversion     \

LIBS        += ../vkapi.so -lcurlpp -lsimdjson -lcares
INCLUDEPATH += ../cpp_vk_lib

HEADERS     += \
            include/3rd_party/string_utils.hpp                    \
            include/long_poll_api/long_poll_api.hpp               \
            include/event_handler/message_event_handler.hpp       \
            include/commands/base_command.hpp                     \
            \
            include/commands/pics.hpp                             \
            include/commands/videos.hpp                           \
            include/commands/about.hpp                            \
            include/commands/cat.hpp                              \
            include/commands/chat_name.hpp                        \
            include/commands/complete.hpp                         \
            include/commands/currency.hpp                         \
            include/commands/docs.hpp                             \
            include/commands/exit.hpp                             \
            include/commands/host.hpp                             \
            include/commands/ip.hpp                               \
            include/commands/kick.hpp                             \
            include/commands/mandelbrot_set.hpp                   \
            include/commands/online.hpp                           \
            include/commands/photo_loader.hpp                     \
            include/commands/role.hpp                             \
            include/commands/stat.hpp                             \
            include/commands/who.hpp                              \

SOURCES     += \
            src/3rd_party/string_utils.cpp                        \
            src/event_handler/message_event_handler.cpp           \
            src/long_poll_api/long_poll_api.cpp                   \
            \
            src/commands/pics.cpp                                 \
            src/commands/videos.cpp                               \
            src/commands/about.cpp                                \
            src/commands/cat.cpp                                  \
            src/commands/chat_name.cpp                            \
            src/commands/complete.cpp                             \
            src/commands/currency.cpp                             \
            src/commands/docs.cpp                                 \
            src/commands/exit.cpp                                 \
            src/commands/host.cpp                                 \
            src/commands/ip.cpp                                   \
            src/commands/kick.cpp                                 \
            src/commands/mandelbrot_set.cpp                       \
            src/commands/online.cpp                               \
            src/commands/photo_loader.cpp                         \
            src/commands/role.cpp                                 \
            src/commands/stat.cpp                                 \
            src/commands/who.cpp                                  \
            main.cpp                                              \

RESOURCES +=
