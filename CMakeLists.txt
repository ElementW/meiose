cmake_minimum_required(VERSION 3.1)
project(meiose CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(MEIOSE_PROFILE FALSE CACHE BOOL "Enable profiling in meiose's tests")
if(MEIOSE_PROFILE)
  include(CodeCoverage)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COVERAGE_COMPILER_FLAGS}")
endif()

include_directories("./include")

set(CMAKE_CXX_STANDARD 14)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

  set(MEIOSE_OPTIMIZE FALSE CACHE BOOL "Enable compiler optimization")
  if(MEIOSE_OPTIMIZE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
  endif()
endif()

set(HEADER_FILES
  include/meiose/debug.hpp
  include/meiose/endian.hpp
  include/meiose/exceptions.hpp
  include/meiose/meiose.hpp
  include/meiose/json.hpp
  include/meiose/msgpack.hpp
  include/meiose/variant.hpp
)

set(SOURCE_FILES
  src/debug.cpp
  src/variant.cpp
  src/json.cpp
  src/msgpack.cpp
)

set(MEIOSE_BUILD_SHARED FALSE CACHE BOOL "Build shared library")
if(MEIOSE_BUILD_SHARED)
  add_library(meiose SHARED ${SOURCE_FILES} ${HEADER_FILES})
endif()

set(MEIOSE_BUILD_STATIC TRUE CACHE BOOL "Build static library")
if(MEIOSE_BUILD_STATIC)
  add_library(meiose-static STATIC ${SOURCE_FILES} ${HEADER_FILES})
endif()

set(MEIOSE_TESTS FALSE CACHE BOOL "Enable tests")
if(MEIOSE_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
