cmake_minimum_required(VERSION 3.14)

project(statsTests LANGUAGES CXX)

if(PROJECT_IS_TOP_LEVEL)
  find_package(stats REQUIRED)
endif()

function(add_unittest NAME)
  add_executable("test_${NAME}" "src/${NAME}.cpp")
  target_link_libraries("test_${NAME}" PRIVATE stats::stats)
  target_compile_features("test_${NAME}" PRIVATE cxx_std_17)
  add_test("test_${NAME}" "test_${NAME}")
endfunction()

add_unittest(mean)
add_unittest(geometric_mean)
add_unittest(harmonic_mean)
add_unittest(variance)
