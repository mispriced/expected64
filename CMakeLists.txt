cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    expected64
    VERSION 0.1.0
    DESCRIPTION "Short description"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES NONE
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(expected64_expected64 INTERFACE)
add_library(expected64::expected64 ALIAS expected64_expected64)

set_property(
    TARGET expected64_expected64 PROPERTY
    EXPORT_NAME expected64
)

target_include_directories(
    expected64_expected64 ${warning_guard}
    INTERFACE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_compile_features(expected64_expected64 INTERFACE cxx_std_17)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT expected64_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of expected64"
  )
endif()

include(cmake/dev-mode.cmake)
