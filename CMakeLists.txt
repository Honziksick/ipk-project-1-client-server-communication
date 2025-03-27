################################################################################
#                                                                              #
# Project:      OMEGA L4 Scanner                                               #
# University:   Faculty of Information Technology, BUT                         #
# Subject:      IPK: Computer Communications and Networks                      #
#                                                                              #
# File:         CMakeLists.txt                                                 #
# Author:       Jan Kalina <xkalinj00>                                         #
#                                                                              #
# Created:      12.03.2025                                                     #
# Last edit:    23.03.2025                                                     #
#                                                                              #
# Description:  This file contains the CMake configuration for the OMEGA L4    #
#               Scanner project. It sets up the project, specifies the C++20   #
#               standard, collects source files, creates a static library,     #
#               defines the main executable, and configures testing with       #
#               Google Test.                                                   #
#                                                                              #
################################################################################

################################################################################
#                                                                              #
#                        BASIC CMakeLists.txt SETTINGS                         #
#                                                                              #
################################################################################

# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.15)

# Set the project name (source location) and version
project(src LANGUAGES CXX)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compile options based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -Wall -Werror -Wextra -pedantic)
elseif(CMAKE_BUILD_TYPE STREQUAL "Test")
    add_compile_options(-g -fsanitize=address -Wall -Werror -Wextra -pedantic)
    add_link_options(-fsanitize=address)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3 -DNDEBUG)
endif()


################################################################################
#                                                                              #
#                             BUILD CONFIGURATION                              #
#                                                                              #
################################################################################

# Ensure the existence of the build directory
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

# Set the output directory for the app release binary (ipk-l4-scan)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/..")

# Include necessary directories (e.g., libpcap, src)
include_directories(src)

# Collect all source files in the 'src' directory
file(GLOB_RECURSE ALL_SOURCES CONFIGURE_DEPENDS
     src/*.cpp
     src/*.hpp
     )

# Remove main.cpp to avoid including it in the library
list(REMOVE_ITEM ALL_SOURCES
     "${CMAKE_CURRENT_SOURCE_DIR}/src/App/main.cpp"
     )

###                                                                     ###
#  Build library from project files (ipk-l4-scan-lib) (except 'main.cpp') #
###                                                                     ###

# Create a static library from the source files
add_library(ipk-l4-scan-lib STATIC ${ALL_SOURCES})
target_include_directories(ipk-l4-scan-lib PUBLIC src)

# Link with libpcap and libnet manually.
target_link_libraries(ipk-l4-scan-lib PUBLIC pcap net)


###                                  ###
#  Build main executabe (ipk-l4-scan)  #
###                                  ###

add_executable(ipk-l4-scan src/App/main.cpp)
target_link_libraries(ipk-l4-scan
                      PRIVATE
                      ipk-l4-scan-lib
                      )

target_include_directories(ipk-l4-scan PRIVATE ipk-l4-scan-lib)


################################################################################
#                                                                              #
#                              TEST CONFIGURATION                              #
#                                                                              #
################################################################################

enable_testing()

# Fetch Google Test framework
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        main
        )
FetchContent_MakeAvailable(googletest)

# Find all test source files
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS test/*.cpp test/*.hpp)


###                                 ###
#  Build individual test executables  #
###                                 ###

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_link_libraries(${TEST_NAME}
                          PRIVATE
                          ipk-l4-scan-lib
                          gtest
                          gtest_main
                          )
    target_link_options(${TEST_NAME} PRIVATE
                        -Wl,--wrap=getifaddrs
                        -Wl,--wrap=freeifaddrs
                        )
    set_target_properties(${TEST_NAME} PROPERTIES
                          RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/bin"
                          )
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()

###                              ###
#  Build combined test executable  #
###                              ###

add_executable(ipk-l4-scan-test ${TEST_SOURCES})
target_link_libraries(ipk-l4-scan-test
                      PRIVATE
                      ipk-l4-scan-lib
                      gtest
                      gtest_main
                      )
target_link_options(ipk-l4-scan-test PRIVATE
                    -Wl,--wrap=getifaddrs
                    -Wl,--wrap=freeifaddrs
                    )
set_target_properties(ipk-l4-scan-test PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/bin"
                      )
add_test(NAME ipk-l4-scan-test COMMAND ipk-l4-scan-test)

### end of file CMakeLists.txt ###
