cmake_minimum_required(VERSION 3.14)

project(expected64Tests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(expected64 REQUIRED)
  enable_testing()
endif()

# ---- Tests ----

add_executable(expected64_test source/expected64_test.cpp)
target_link_libraries(expected64_test PRIVATE expected64::expected64)
target_compile_features(expected64_test PRIVATE cxx_std_17)

add_test(NAME expected64_test COMMAND expected64_test)

# ---- End-of-file commands ----

add_folders(Test)
