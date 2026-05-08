cmake_minimum_required(VERSION 3.18)

project(LosCssDrogon LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Drogon CONFIG REQUIRED)

file(
    GLOB SRC_FILES
    ${CMAKE_SOURCE_DIR}/src/main.cc
    ${CMAKE_SOURCE_DIR}/src/*.cc
    ${CMAKE_SOURCE_DIR}/src/*.h
    ${CMAKE_SOURCE_DIR}/src/controllers/*.cc
    ${CMAKE_SOURCE_DIR}/src/controllers/*.h
    ${CMAKE_SOURCE_DIR}/src/controllers/v1/*.cc
    ${CMAKE_SOURCE_DIR}/src/controllers/v1/*.h
    ${CMAKE_SOURCE_DIR}/src/models/*.cc
    ${CMAKE_SOURCE_DIR}/src/models/*.h
    ${CMAKE_SOURCE_DIR}/src/filters/*.cc
    ${CMAKE_SOURCE_DIR}/src/filters/*.h
    ${CMAKE_SOURCE_DIR}/src/plugins/*.cc
    ${CMAKE_SOURCE_DIR}/src/plugins/*.h
    ${CMAKE_SOURCE_DIR}/src/services/*.cc
    ${CMAKE_SOURCE_DIR}/src/services/*.h
    ${CMAKE_SOURCE_DIR}/src/utils/*.cc
    ${CMAKE_SOURCE_DIR}/src/utils/*.h
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)

configure_file(
    ${CMAKE_SOURCE_DIR}/config.json
    ${CMAKE_BINARY_DIR}/config.json
    COPYONLY
)