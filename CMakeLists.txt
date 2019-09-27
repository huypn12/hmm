cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(hmm)

# Load CMake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/CMake)
include(cotire)

find_package(Eigen3 REQUIRED NO_MODULE)

# Build static lib
set(SOURCES
  src/markov_random.cc
  src/dtmc.cc
  src/hmm.cc
)
add_library(hmm ${SOURCES})
target_include_directories(hmm PRIVATE include)
target_link_libraries(hmm Eigen3::Eigen)
cotire(hmm)

# Unittest
add_executable(test_hmm test/test_hmm.cpp)
target_include_directories(test_hmm SYSTEM PRIVATE include third_party/catch2)
target_link_libraries(test_hmm hmm)
cotire(test_hmm)
enable_testing()
add_test(NAME hmm COMMAND test_hmm)

