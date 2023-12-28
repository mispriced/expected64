cmake_minimum_required(VERSION 3.14)

project(expected64Benchmarks LANGUAGES CXX)

include(${CMAKE_SOURCE_DIR}/cmake/project-is-top-level.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/folders.cmake)

add_executable(expected64_benchmark
        bench.cpp
        )

target_include_directories(expected64_benchmark PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        )

target_link_libraries(expected64_benchmark
        Catch2::Catch2WithMain
        )

target_compile_features(expected64_benchmark PRIVATE cxx_std_20)
add_custom_target(run-benchmarks
        COMMAND expected64_benchmark
        DEPENDS expected64_benchmark
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running benchmarks..."
)

add_folders(Benchmark)