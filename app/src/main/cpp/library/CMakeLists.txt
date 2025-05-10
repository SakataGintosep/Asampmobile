project(Library)

file(GLOB INCLUDE_FILES
        bass/*.h
        encoding/*.h
        imgui/*.h
        memory/*.h
        NvEvent/*.h
        obfuscator/*.h
        opus/*.h
        RakNet/*.h
        RakNet/**/*.h
        rapidjson/*.h
        rapidjson/**/*.h)

file(GLOB SOURCE_FILES
        bass/*.c*
        encoding/*.c*
        imgui/*.c*
        memory/*.c*
        NvEvent/*.c*
        obfuscator/*.c*
        opus/*.c*
        RakNet/*.c*
        RakNet/**/*.c*
        rapidjson/*.c*
        rapidjson/**/*.c*)

add_library(${PROJECT_NAME} INTERFACE)

target_sources(${PROJECT_NAME} INTERFACE
        ${INCLUDE_FILES}
        ${SOURCE_FILES})

add_library(Opus STATIC IMPORTED)
set_target_properties(Opus PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/library/opus/libopus.a)

target_link_libraries(${PROJECT_NAME} INTERFACE
        dobby
        KittyMemory
        Opus)

target_include_directories(${PROJECT_NAME} INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")

target_include_directories(${PROJECT_NAME} INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>")

add_subdirectory(memory/Dobby)
add_subdirectory(memory/KittyMemory)
add_subdirectory(ttmath)
