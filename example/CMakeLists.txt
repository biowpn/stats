cmake_minimum_required(VERSION 3.14)

project(statsExamples CXX)

if(PROJECT_IS_TOP_LEVEL)
  find_package(stats REQUIRED)
endif()

function(add_example NAME)
  add_executable("example_${NAME}" "src/${NAME}.cpp")
  target_link_libraries("example_${NAME}" PRIVATE stats::stats)
  target_compile_features("example_${NAME}" PRIVATE cxx_std_17)
endfunction()

add_example(example1)
