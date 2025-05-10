project(Alyn_SAMPMOBILE)

file(GLOB INCLUDE_FILES
        *.h
        game/*.h
        game/**/*.h
        net/*.h
        ui/*.h
        ui/**/*.h
        ui/**/**/*.h
        voice/*.h
        voice/**/*.h
        voice/**/**/*.h)

file(GLOB SOURCE_FILES
        *.c*
        game/*.c*
        game/**/*.c*
        net/*.c*
        ui/*.c*
        ui/**/*.c*
        ui/**/**/*.c*
        voice/*.c*
        voice/**/*.c*
        voice/**/**/*.c*)

add_library(${PROJECT_NAME} SHARED
        ${INCLUDE_FILES}
        ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE
        Library
        log
        z
        android
        OpenSLES)

target_compile_options(${PROJECT_NAME} PRIVATE
        -w
        -fexceptions)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        RAKSAMP_CLIENT)

set_property(TARGET ${PROJECT_NAME} PROPERTY ENABLE_EXPORTS ON)
