project(scramjetpeer_test LANGUAGES CXX)

cmake_minimum_required(VERSION 3.13)

find_package(GTest REQUIRED)

enable_testing()

#add_executable(
	#  timeconvert.test
	#  timeconvert_test.cpp
	#)



get_property(targets DIRECTORY "${CMAKE_CURRENT_LIST_DIR}" PROPERTY BUILDSYSTEM_TARGETS)
foreach(tgt ${targets})
  get_target_property(target_type ${tgt} TYPE)
  if (target_type STREQUAL "EXECUTABLE")
    target_link_libraries(${tgt} ${CMAKE_THREAD_LIBS_INIT} hbm::hbm GTest::GTest GTest::Main) 
    if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
        target_link_libraries(${tgt} gcov)
    endif()

    set_target_properties(${tgt} PROPERTIES
      LINK_FLAGS "${EXTRA_LINK_FLAGS}"
      CXX_STANDARD_REQUIRED ON
      CXX_STANDARD 14
      CXX_EXTENSIONS OFF
    )
    add_test(NAME hbm.sys.${tgt} COMMAND ${tgt}
      --report_level=no
      --log_level=all
    )
  endif()
endforeach()



