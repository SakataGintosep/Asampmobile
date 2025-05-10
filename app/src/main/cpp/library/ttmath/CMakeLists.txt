set(PROJECT ttmath)
add_library(ttmath INTERFACE)

target_include_directories(ttmath INTERFACE .)

file(GLOB_RECURSE ttmath_source_list "*.h")

set_property(TARGET ttmath PROPERTY SOURCES ${ttmath_source_list})

set_property(TARGET ttmath PROPERTY OUTPUT_NAME ttmath)
