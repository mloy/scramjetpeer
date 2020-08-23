cmake_minimum_required(VERSION 3.13)

project(scramjetpeer_all)


option(FEATURE_POST_BUILD_UNITTEST  "Automatically run unit-tests as a post build step" ON)


#find_package(scramjet)

add_subdirectory("lib")

add_subdirectory("tool")

if(FEATURE_POST_BUILD_UNITTEST)
  enable_testing()
  add_subdirectory("test")
endif()
