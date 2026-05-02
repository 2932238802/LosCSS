cmake_minimum_required(VERSION 3.18)

project(LosCssDrogon LANGUAGE CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(drogon CONFIG REQUIRED)

file(
    GLOB SRC_FILES
    main.cc
    ${CMAKE_SOURCE_DIR}/*.h
    ${CMAKE_SOURCE_DIR}/*.cc
    ${CMAKE_SOURCE_DIR}/controllers/*.h
    ${CMAKE_SOURCE_DIR}/controllers/*.cc
    ${CMAKE_SOURCE_DIR}/models/*.h
    ${CMAKE_SOURCE_DIR}/models/*.cc
    ${CMAKE_SOURCE_DIR}/filters/*.h
    ${CMAKE_SOURCE_DIR}/plugins/*.cc
    ${CMAKE_SOURCE_DIR}/plugins/*.h
    ${CMAKE_SOURCE_DIR}/services/*.cc
    ${CMAKE_SOURCE_DIR}/services/*.h
    ${CMAKE_SOURCE_DIR}/utils/*.cc
    ${CMAKE_SOURCE_DIR}/utils/*.h
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE Drogon:Drogon)
