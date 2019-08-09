cmake_minimum_required(VERSION 3.0)

project(libhmm)

# Dependencies check
find_package(Eigen3 REQUIRED NO_MODULE)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# Build
include_directories(include)
file(GLOB SOURCES "src/*.cpp")
add_library(libhmm ${SOURCES})
target_link_libraries(libhmm Eigen3::Eigen)

# Test
add_executable(test_mc_random test/test_mc_random.cpp)
target_link_libraries(test_mc_random libhmm Boost::unit_test_framework)
add_executable(test_dtmc test/test_dtmc.cpp)
target_link_libraries(test_dtmc libhmm Boost::unit_test_framework)
add_executable(test_hmm test/test_hmm.cpp)
target_link_libraries(test_dtmc libhmm Boost::unit_test_framework)


