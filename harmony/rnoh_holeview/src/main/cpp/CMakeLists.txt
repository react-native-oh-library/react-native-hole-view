cmake_minimum_required(VERSION 3.13)
set(CMAKE_VERBOSE_MAKEFILE on)

set(rnoh_holeview_generated_dir "./_react_native_oh_tpl_react_native_hole_view")

file(GLOB_RECURSE rnoh_holeview_generated_SRC "${rnoh_holeview_generated_dir}/**/*.cpp")

file(GLOB rnoh_holeview_SRC CONFIGURE_DEPENDS *.cpp )

add_library(rnoh_holeview SHARED ${rnoh_holeview_SRC} ${rnoh_holeview_generated_SRC})

target_include_directories(rnoh_holeview PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${rnoh_holeview_generated_dir})

target_link_libraries(rnoh_holeview PUBLIC rnoh)
