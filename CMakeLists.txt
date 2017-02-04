cmake_minimum_required(VERSION 3.2.2)

# Project name
project(TeamSpeak-c-api)

# CXX flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Build architecture
if(NOT DEFINED BUILD_x86)
  set(BUILD_ARCH "x86_64")
  set(BUILD_ARCH_OPTION "-m64")
else()
  set(BUILD_ARCH "x86")
  set(BUILD_ARCH_OPTION "-m32")
endif()

# CXX buld architecture
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BUILD_ARCH_OPTION}")

set(LIBRARY_NAME teamspeak-api-${BUILD_ARCH})

# Include directories
include_directories(
  src/includes
)

# Soruce files
add_library(${LIBRARY_NAME} SHARED
  src/ts3ObjectProperties.cpp
  src/serverStructs.cpp
  src/Server.cpp
  src/functions.cpp
  src/Client.cpp
  src/Group.cpp
  src/Channel.cpp
  src/Permission.cpp
)

# Executable source files
set(SRCS
  src/main.cpp
)

# Executable file
add_executable(TeamSpeak-c-api ${SRCS})
target_link_libraries(${LIBRARY_NAME} pthread)
target_link_libraries(TeamSpeak-c-api pthread)
target_link_libraries(TeamSpeak-c-api ${LIBRARY_NAME})

# Unit Tests
enable_testing()


add_executable(Test unitTests/connection_test.cpp)
target_link_libraries(Test ${LIBRARY_NAME})
target_link_libraries(Test pthread)

add_executable(split_tools_test unitTests/split_tools_test.cpp)
target_link_libraries(split_tools_test ${LIBRARY_NAME})
target_link_libraries(split_tools_test pthread)

add_test(NAME Connection_test COMMAND Test)
add_test(NAME split_tools_test COMMAND split_tools_test)