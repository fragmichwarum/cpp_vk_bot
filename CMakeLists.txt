cmake_minimum_required(VERSION 3.5)

project(VK LANGUAGES CXX)

include_directories(src)
include_directories(include)
include_directories(src/Commands)
include_directories(include/Commands)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS -flto)

set(HEADERS
  include/EventHandler.hpp
  include/ICommand.hpp
  include/Logger.hpp
  include/LongPollListener.hpp
  include/MessageEventHandler.hpp
  include/Network.hpp
  include/PostEventHandler.hpp
  include/Repository/Repository.hpp
  include/Repository/SQLiteRepository.hpp
  include/Utility.hpp
  include/VkAPI.hpp
  include/JsonUtils.hpp
  include/VKError.hpp
)
set(COMMAND_HEADERS
  include/Commands/About.hpp
  include/Commands/Cat.hpp
  include/Commands/ChatName.hpp
  include/Commands/Complete.hpp
  include/Commands/Currency.hpp
  include/Commands/Document.hpp
  include/Commands/Exit.hpp
  include/Commands/Host.hpp
  include/Commands/Ip.hpp
  include/Commands/Kick.hpp
  include/Commands/Online.hpp
  include/Commands/Picture.hpp
  include/Commands/Ping.hpp
  include/Commands/Role.hpp
  include/Commands/Stat.hpp
  include/Commands/Video.hpp
  include/Commands/Who.hpp
)
set(SOURCES
  src/EventHandler.cpp
  src/main.cpp
  src/LongPollListener.cpp
  src/Logger.cpp
  src/MessageEventHandler.cpp
  src/Network.cpp
  src/PostEventHandler.cpp
  src/Repository/SQLiteRepository.cpp
  src/Utility.cpp
  src/VkAPI.cpp
  src/JsonUtils.cpp
)
set(COMMAND_SOURCES
  src/Commands/About.cpp
  src/Commands/Cat.cpp
  src/Commands/ChatName.cpp
  src/Commands/Complete.cpp
  src/Commands/Currency.cpp
  src/Commands/Document.cpp
  src/Commands/Exit.cpp
  src/Commands/Host.cpp
  src/Commands/Ip.cpp
  src/Commands/Kick.cpp
  src/Commands/Online.cpp
  src/Commands/Picture.cpp
  src/Commands/Ping.cpp
  src/Commands/Role.cpp
  src/Commands/Stat.cpp
  src/Commands/Video.cpp
  src/Commands/Who.cpp
)

add_executable(VK ${HEADERS} ${SOURCES} ${COMMAND_HEADERS} ${COMMAND_SOURCES})
target_link_libraries(VK simdjson curl curlpp SQLiteCpp cares)
target_compile_options(VK PRIVATE -Wall -Wextra -pedantic)
