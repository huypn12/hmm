cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(hmm)

# Load CMake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/CMake)
include(cotire)

find_package(Eigen3 REQUIRED NO_MODULE)

# Build static lib
include_directories(include)
set(SOURCES
  src/mc_random.cpp
  src/dtmc.cpp
  src/hmm.cpp
)
add_library(hmm ${SOURCES})
target_link_libraries(hmm Eigen3::Eigen)
cotire(hmm)

# Unittest
enable_testing()
include_directories(third_party/catch2)
add_executable(test_hmm test/test_hmm.cpp)
target_link_libraries(test_hmm hmm)
add_test(NAME hmm COMMAND test_hmm)

