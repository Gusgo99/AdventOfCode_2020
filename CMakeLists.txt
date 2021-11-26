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

set(
	FILES
	"day1_report_repair"
	"day2_password_philosophy"
	"day3_toboggan_trajectory"
	"day4_passport_processing"
	"day5_binary_boarding"
	"day6_custom_customs"
	"day7_handy_haversacks"
	"day8_handheld_halting"
	"day9_encoding_error"
	"day10_adapter_array"
	"day11_seating_system"
	"day12_rain_risk"
	"day13_shuttle_search"
	"day14_docking_data"
	"day15_rambunctious_recitation"
	"day16_ticket_translation"
	"day17_conway_cubes"
	"day18_operation_order"
	"day19_monster_message"
	"day21_allergen_assessment"
	"day22_crab_combat"
	"day23_crab_cups"
	"day24_lobby_layout"
	"day25_combo_breaker"
)

foreach(FILE ${FILES})
	add_executable(${FILE} "${PROJECT_SOURCE_DIR}/src/${FILE}.cpp")
	set_standard_cxx_flags(${FILE} PRIVATE)
	set_standard_linker_flags(${FILE} PRIVATE)
endforeach()