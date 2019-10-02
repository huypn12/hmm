cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(hmm)

# Load CMake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/CMake)
include(cotire)

find_package(Eigen3 REQUIRED NO_MODULE)

# Static lib
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

# Examples
file(COPY examples/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(reduction examples/reduction.cc)
target_include_directories(reduction PRIVATE include)
target_link_libraries(reduction hmm)
cotire(reduction)

add_executable(simulation examples/simulation.cc)
target_include_directories(simulation PRIVATE include)
target_link_libraries(simulation hmm)
cotire(reduction)

