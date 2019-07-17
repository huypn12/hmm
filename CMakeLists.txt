cmake_minimum_required(VERSION 3.13)

project(hmm)

set(CMAKE_CXX_STANDARD 17)

set(HMM_SOURCE_FILES src/dtmc.cpp)
include_directories(./include)
add_library(hmm STATIC ${HMM_SOURCE_FILES})

add_executable(test_hmm test/test_dtmc.cpp)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS unit_test_framework)
target_link_libraries(test_hmm ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
