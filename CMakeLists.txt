cmake_minimum_required(VERSION 3.2)
project (xxhashct)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable (xxhashct_tests test.cpp)

find_library(XXHASH_LIBRARY xxhash)
find_path(XXHASH_INCLUDE_DIR xxhash.h)

if (XXHASH_LIBRARY AND XXHASH_INCLUDE_DIR)
  target_link_libraries (xxhashct_tests ${XXHASH_LIBRARY})
  target_include_directories (xxhashct_tests PRIVATE ${XXHASH_INCLUDE_DIR})
  target_compile_definitions (xxhashct_tests PRIVATE HAVE_XXHASH)
else ()
  message (WARNING "xxhash library not found. Will only test against hardcoded hash values.")
endif ()
