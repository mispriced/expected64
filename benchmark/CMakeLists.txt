cmake_minimum_required(VERSION 3.14)

project(expected64Benchmarks LANGUAGES CXX)

include(${CMAKE_SOURCE_DIR}/cmake/project-is-top-level.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/folders.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/fetch-nanobench.cmake)

add_executable(expected64_benchmark_catch2
        bench.cpp
        )

target_include_directories(expected64_benchmark_catch2 PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/3rdparty
        )

target_link_libraries(expected64_benchmark_catch2
        Catch2::Catch2WithMain
        )

target_compile_features(expected64_benchmark_catch2 PRIVATE cxx_std_20)
add_custom_target(run-benchmarks
        COMMAND expected64_benchmark_catch2
        DEPENDS expected64_benchmark_catch2
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running benchmarks..."
)

add_executable(expected64_benchmark_nanobench
        bench_nano.cpp
)

target_include_directories(expected64_benchmark_nanobench PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/3rdparty
        )

target_link_libraries(expected64_benchmark_nanobench
        nanobench
        )
target_compile_features(expected64_benchmark_nanobench PRIVATE cxx_std_23)

add_folders(Benchmark)