cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(hmm)

# Dependencies check
include_directories(external/eigen)
include_directories(external/catch2/single_include)

# Build static lib
include_directories(include)
set(SOURCES
  src/mc_random.cpp
  src/dtmc.cpp
  src/hmm.cpp
)
add_library(hmm ${SOURCES})

ENABLE_TESTING()

# Test some random behaviour
add_executable(test_mc_random test/test_mc_random.cpp)
target_link_libraries(test_mc_random hmm)

# Test dtmc with simple trace
add_executable(test_dtmc test/test_dtmc.cpp)
target_link_libraries(test_dtmc hmm)

# Test Forward-Backward algo
add_executable(test_hmm test/test_hmm.cpp)
target_link_libraries(test_hmm hmm)

add_test(libhmm test_mc_random test_dtmc test_hmm)
