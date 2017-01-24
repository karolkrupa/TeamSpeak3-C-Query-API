cmake_minimum_required(VERSION 3.2.2)

project(TeamSpeak-c-api)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
  src/includes
)

add_library(teamspeak-api SHARED
  src/ts3ObjectProperties.cpp
  src/serverStructs.cpp
  src/Server.cpp
  src/functions.cpp
  src/Client.cpp
  src/Group.cpp
  src/Channel.cpp
  src/Permission.cpp
)

set(SRCS
  src/main.cpp
)

add_executable(TeamSpeak-c-api ${SRCS})
target_link_libraries(teamspeak-api pthread)
target_link_libraries(TeamSpeak-c-api pthread)
target_link_libraries(TeamSpeak-c-api teamspeak-api)
