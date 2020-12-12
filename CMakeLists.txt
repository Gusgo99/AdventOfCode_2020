cmake_minimum_required(VERSION "3.17.0")

project("Advent_of_code")

# Set language version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include("${PROJECT_SOURCE_DIR}/cmake/standard_warnings_and_optimizations.cmake")

set(INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

add_definitions("-DUNICODE")
add_definitions("-D_UNICODE")

if((CMAKE_CXX_COMPILER_ID STREQUAL GNU) AND (CMAKE_BUILD_TYPE STREQUAL Debug))
	add_definitions("-D_GLIBCXX_DEBUG")
endif()

include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/include")

add_executable(day1_report_repair "${PROJECT_SOURCE_DIR}/src/day1_report_repair.cpp")
add_executable(day2_password_philosophy "${PROJECT_SOURCE_DIR}/src/day2_password_philosophy.cpp")
add_executable(day3_toboggan_trajectory "${PROJECT_SOURCE_DIR}/src/day3_toboggan_trajectory.cpp")
add_executable(day4_passport_processing "${PROJECT_SOURCE_DIR}/src/day4_passport_processing.cpp")
add_executable(day5_binary_boarding "${PROJECT_SOURCE_DIR}/src/day5_binary_boarding.cpp")
add_executable(day6_custom_customs "${PROJECT_SOURCE_DIR}/src/day6_custom_customs.cpp")
add_executable(day9_encoding_error "${PROJECT_SOURCE_DIR}/src/day9_encoding_error.cpp")
add_executable(day10_adapter_array "${PROJECT_SOURCE_DIR}/src/day10_adapter_array.cpp")
add_executable(day11_seating_system "${PROJECT_SOURCE_DIR}/src/day11_seating_system.cpp")

set_standard_cxx_flags(day1_report_repair PUBLIC)
set_standard_cxx_flags(day2_password_philosophy PUBLIC)
set_standard_cxx_flags(day3_toboggan_trajectory PUBLIC)
set_standard_cxx_flags(day4_passport_processing PUBLIC)
set_standard_cxx_flags(day5_binary_boarding PUBLIC)
set_standard_cxx_flags(day6_custom_customs PUBLIC)
set_standard_cxx_flags(day9_encoding_error PUBLIC)
set_standard_cxx_flags(day10_adapter_array PUBLIC)
set_standard_cxx_flags(day11_seating_system PUBLIC)

set_standard_linker_flags(day1_report_repair PRIVATE)
set_standard_linker_flags(day2_password_philosophy PRIVATE)
set_standard_linker_flags(day3_toboggan_trajectory PRIVATE)
set_standard_linker_flags(day4_passport_processing PRIVATE)
set_standard_linker_flags(day5_binary_boarding PRIVATE)
set_standard_linker_flags(day6_custom_customs PRIVATE)
set_standard_linker_flags(day9_encoding_error PRIVATE)
set_standard_linker_flags(day10_adapter_array PRIVATE)
set_standard_linker_flags(day11_seating_system PRIVATE)
