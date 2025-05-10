cmake_minimum_required(VERSION 3.0)
project(KittyMemory)

option(kITTYMEMORY_DEBUG "Enable KittyMemory debug log" OFF)
option(kNO_KEYSTONE "Disable Keystone" ON)

file(GLOB INCLUDE_FILES
        KittyMemory/*.hpp)

file(GLOB SOURCE_FILES
        KittyMemory/*.cpp)

add_library(${PROJECT_NAME} STATIC
        ${INCLUDE_FILES}
        ${SOURCE_FILES})

if (kITTYMEMORY_DEBUG)
    target_compile_definitions(${PROJECT_NAME} PUBLIC
            kITTYMEMORY_DEBUG)
endif ()

if (kNO_KEYSTONE)
    target_compile_definitions(${PROJECT_NAME} PUBLIC
            kNO_KEYSTONE)
else ()
    target_link_libraries(${PROJECT_NAME} PUBLIC
            keystone)
endif ()

target_include_directories(${PROJECT_NAME}
        PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/KittyMemory>")
