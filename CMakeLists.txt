cmake_minimum_required(VERSION 3.5)
project(septal C CXX)

set(CMAKE_C_STANDARD 99)

option(SEPTAL_BUILD_TESTS "Build septal tests" OFF)

function(septal_get_submodules)
    find_package(Git QUIET)
    if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
        # Update submodules as needed
        option(GIT_SUBMODULE "Check submodules during build" ON)
        if(GIT_SUBMODULE)
            message(STATUS "Submodule update")
            execute_process(
                    COMMAND ${GIT_EXECUTABLE}
                            submodule update --init --recursive
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    RESULT_VARIABLE INIT_RESULT)
            if(NOT INIT_RESULT EQUAL "0")
                message(
                        FATAL_ERROR
                        "git submodule update --init failed with "
                        "${GIT_SUBMOD_RESULT}, please checkout submodules")
            endif()
        endif()
    endif()

    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/gtest/CMakeLists.txt")
        message(
                FATAL_ERROR
                "Failed to clone submodules. "
                "Please update manually or try again.")
    endif()
endfunction()

septal_get_submodules()

if(NOT TARGET gtest)
    add_subdirectory(third_party/gtest)
endif()

add_library(septal INTERFACE)
target_include_directories(septal INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Add test target.
if(NOT SEPTAL_BUILD_TESTS)
    set(TEST_EXCLUDE "EXCLUDE_FROM_ALL")
endif()
add_executable(test_septal ${TEST_EXCLUDE} test_septal.cc)
target_link_libraries(test_septal septal gtest gtest_main)
