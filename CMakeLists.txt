cmake_minimum_required(VERSION 3.10)

project(spancopy VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "-Wall -O3")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/externals
)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/spanner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

if (APPLE)
    target_link_libraries(
        ${PROJECT_NAME}
        -mmacosx-version-min=10.15
    )
else()
    target_link_libraries(
        ${PROJECT_NAME}
        -static
    )
endif()
