# This is the top level CMakeLists.txt file - it can be used to store
# project wide settings.

CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)
PROJECT(Network)

SET(PROJECT_MAJOR_VERSION 0)
SET(PROJECT_MINOR_VERSION 1)
SET(PROJECT_VERSION ${PROJECT_MAJOR_VERSION}.${PROJECT_MINOR_VERSION})

SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

SET(CMAKE_VERBOSE_MAKEFILE ON)

# Third-party dependencies
# TODO: ZMQ should be re-factored to make use of the CMAKE package system
# Should also look to remove the THIRDPARTY_ROOT requirement.
SET(THIRDPARTY_ROOT ${PROJECT_SOURCE_DIR}/../thirdparty/)

SET(THIRDPARTY_ZMQ_INCLUDE ${THIRDPARTY_ROOT}/cppzmq
                           ${THIRDPARTY_ROOT}/ZeroMQ-4.0.4/build/include)
SET(THIRDPARTY_ZMQ_LIB_DIR ${THIRDPARTY_ROOT}/ZeroMQ-4.0.4/build/lib)

SET(Qt5_DIR ${THIRDPARTY_ROOT}/Qt/5.3/clang_64/lib/cmake/Qt5)
SET(Qt5Widgets_DIR ${THIRDPARTY_ROOT}/Qt/5.3/clang_64/lib/cmake/Qt5Widgets)
SET(Qt5OpenGL_DIR ${THIRDPARTY_ROOT}/Qt/5.3/clang_64/lib/cmake/Qt5OpenGL)

# Project wide-includes
INCLUDE_DIRECTORIES(src)

# Build Settings
SET(CMAKE_CXX_FLAGS "-Wall -Werror -pedantic -std=gnu++11 -stdlib=libc++")

# Build the project
ADD_SUBDIRECTORY(src)
