cmake_minimum_required(VERSION 3.13)

project("scramjetpeer_tool")

include_directories(../../lib/include)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads)

SET(PROTOCOLVERSION_SOURCE
    protocolversion.cpp
)

add_executable(protocolversion
    protocolversion.cpp
)

target_link_libraries( protocolversion
    ${CMAKE_THREAD_LIBS_INIT}
    scramjetpeer
)

install(TARGETS protocolversion RUNTIME DESTINATION bin)
