cmake_minimum_required(VERSION 3.14)

project(
    stats
    DESCRIPTION "Basic Statistics"
)


# ---- Declare library ----

add_library(stats_stats INTERFACE)
add_library(stats::stats ALIAS stats_stats)

set_property(
    TARGET stats_stats PROPERTY
    EXPORT_NAME stats
)

target_include_directories(
    stats_stats ${warning_guard}
    INTERFACE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_compile_features(stats_stats INTERFACE cxx_std_23)

# ---- Install rules ----

option(BUILD_EXAMPLES "Build examples tree." "${stats_DEVELOPER_MODE}")
if(BUILD_EXAMPLES)
  add_subdirectory(example)
endif()

option(BUILD_TESTS "Build tests tree." "${stats_DEVELOPER_MODE}")
if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()
