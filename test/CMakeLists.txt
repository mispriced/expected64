cmake_minimum_required(VERSION 3.14)

project(expected64Tests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(expected64 REQUIRED)
  find_package(Catch2 REQUIRED)
  enable_testing()
endif()

# ---- Tests ----

add_executable(expected64_test src/expected64_test.cpp)
target_include_directories(expected64_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        )
target_link_libraries(expected64_test 
  PRIVATE expected64::expected64
  Catch2::Catch2WithMain)
target_compile_features(expected64_test PRIVATE cxx_std_20)

add_test(NAME expected64_test COMMAND expected64_test)

# ---- End-of-file commands ----

add_folders(Test)
