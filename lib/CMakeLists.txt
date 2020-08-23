cmake_minimum_required(VERSION 3.13)



project(scramjetpeer VERSION 0.0.0 LANGUAGES CXX)

find_package(Threads)
find_package(Boost)



include_directories("include")

set( SCRAMJETCPPEERLIB_INTERFACE_HEADERS
	#    include/hbm/communication/ipv4address.h
)


set(SCRAMJETCPPEERLIB_SOURCES
  ${SCRAMJETCPPEERLIB_INTERFACE_HEADERS}
  scramjetpeer.cpp
)

add_library(${PROJECT_NAME} ${SCRAMJETCPPEERLIB_SOURCES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})




set_target_properties(${PROJECT_NAME} PROPERTIES
  VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
  SOVERSION ${PROJECT_VERSION_MAJOR}
  CXX_STANDARD_REQUIRED ON
  CXX_STANDARD 17
  CXX_EXTENSIONS OFF
)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

install(
  DIRECTORY "include/${PROJECT_NAME}"
  DESTINATION include
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${${PROJECT_NAME}_VERSION}
  COMPATIBILITY SameMajorVersion
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION lib/cmake/${PROJECT_NAME})


# Create and install stuff for find_package
install(
    EXPORT ${PROJECT_NAME}
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION lib/cmake/${PROJECT_NAME}
    FILE ${PROJECT_NAME}Config.cmake
)


