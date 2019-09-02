cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

include(PrecompiledHeader.cmake)

project(hmm)

# Dependencies check
include_directories(external/eigen)
include_directories(external/catch2/single_include)

# Build static lib
include_directories(include)
set(SOURCES
  src/pchheader.cpp
  src/mc_random.cpp
  src/dtmc.cpp
  src/hmm.cpp
)
add_library(hmm ${SOURCES})
add_precompiled_header(hmm include/pchheader.hpp FORCEINCLUDE)

# Unit test
add_executable(test_mc_random test/test_mc_random.cpp)
target_link_libraries(test_mc_random hmm)

add_executable(test_dtmc test/test_dtmc.cpp)
target_link_libraries(test_dtmc hmm)

add_executable(test_hmm test/test_hmm.cpp)
target_link_libraries(test_hmm hmm)

ENABLE_TESTING()
add_test(hmm test_mc_random test_dtmc test_hmm)

# Experiment
add_executable(reduction experiment/reduction.cpp)
target_link_libraries(reduction hmm)
