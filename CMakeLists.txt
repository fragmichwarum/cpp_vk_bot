cmake_minimum_required(VERSION 3.5)

project(FactoryVK LANGUAGES CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
include_directories(include/Commands)

find_package( Threads )

set(SOURCES
  src/Curl.cpp
  src/Database.cpp
  src/Invoker.cpp
  src/LongPoll.cpp
  src/Logger.cpp
  src/Utils.cpp
  src/VkAPI.cpp
  main.cpp
)
set(COMMAND_SOURCES
  src/Commands/Cat.cpp
  src/Commands/Currency.cpp
  src/Commands/Github.cpp
  src/Commands/Stat.cpp
  src/Commands/Document.cpp
  src/Commands/Face.cpp
  src/Commands/Picture.cpp
  src/Commands/Video.cpp
  src/Commands/Weather.cpp
)
set(HEADERS
  include/Curl.hpp
  include/Database.hpp
  include/Invoker.hpp
  include/ICommand.hpp
  include/LongPoll.hpp
  include/Logger.hpp
  include/VkError.hpp
  include/VkAPI.hpp
  include/Utils.hpp
)
set(COMMAND_HEADERS
  include/Commands/Cat.hpp
  include/Commands/Currency.hpp
  include/Commands/CRC32.hpp
  include/Commands/Face.hpp
  include/Commands/Github.hpp
  include/Commands/Document.hpp
  include/Commands/Picture.hpp
  include/Commands/Stat.hpp
  include/Commands/Video.hpp
  include/Commands/Weather.hpp
)
set(INTERNAL_LIBS
  lib/src/crc32.cpp
  lib/include/crc32.hpp
  lib/include/Json.hpp
)

add_executable(
  ${PROJECT_NAME}
  ${SOURCES}
  ${HEADERS}
  ${COMMAND_HEADERS}
  ${COMMAND_SOURCES}
  ${INTERNAL_LIBS}
)
target_link_libraries( FactoryVK curl pthread )
