include(cmake/folders.cmake)
include(CTest)

if (BUILD_TESTING)
  set(CMAKE_MODULE_PATH CACHE INTERNAL FORCE)
endif ()

if (BUILD_TESTING)
  Include(FetchContent)
  FetchContent_Declare(
          Catch2
          GIT_TAG v3.4.0
          GIT_REPOSITORY https://github.com/catchorg/Catch2.git
          GIT_SHALLOW TRUE
  )
  FetchContent_MakeAvailable(Catch2)
endif ()

if(BUILD_TESTING)
  add_subdirectory(test)
endif()



option(BUILD_MCSS_DOCS "Build documentation using Doxygen and m.css" OFF)
if(BUILD_MCSS_DOCS)
  include(cmake/docs.cmake)
endif()

option(ENABLE_COVERAGE "Enable coverage support separate from CTest's" OFF)
if(ENABLE_COVERAGE)
  include(cmake/coverage.cmake)
endif()

include(cmake/lint-targets.cmake)
include(cmake/spell-targets.cmake)

add_folders(Project)
