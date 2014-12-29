# This is the top level CMakeLists.txt file - it can be used to store
# project wide settings.

CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(Network)

SET(PROJECT_MAJOR_VERSION 0)
SET(PROJECT_MINOR_VERSION 1)
SET(PROJECT_VERSION ${PROJECT_MAJOR_VERSION}.${PROJECT_MINOR_VERSION})

SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

# Build Settings
SET(NETWORK_CXX_FLAGS "-Wall -Werror -pedantic -std=gnu++11 -stdlib=libc++")
SET(CMAKE_CXX_FLAGS ${NETWORK_CXX_FLAGS})

# Build the project
ADD_SUBDIRECTORY(src)