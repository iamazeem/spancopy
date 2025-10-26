cmake_minimum_required(VERSION 3.10)

project(spancopy)

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

if(CMAKE_CXX_COMPILER MATCHES "mingw")
    find_program(WINDRES_EXECUTABLE x86_64-w64-mingw32-windres)
    if(WINDRES_EXECUTABLE)
        message(STATUS "Found windres: ${WINDRES_EXECUTABLE}")
    else()
        message(FATAL_ERROR "windres not found")
    endif()

    if(DEFINED ENV{TAG})
        set(TAG $ENV{TAG})
    else()
        set(TAG "0.0.0")
    endif()
    message(STATUS "TAG: ${TAG}")

    set(RC_FILE ${CMAKE_CURRENT_SOURCE_DIR}/src/resource.rc)
    set(RC_OUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/resource.o)

    add_custom_command(
        OUTPUT ${RC_OUT_FILE}
        COMMAND ${WINDRES_EXECUTABLE}
            -DVERSION="\\"${TAG}\\""
            ${RC_FILE} -o ${RC_OUT_FILE}
        DEPENDS ${RC_FILE}
        COMMENT "Compiling resources ${RC_FILE}"
        VERBATIM
    )

    add_custom_target(
        RC_OUT_FILE ALL
        DEPENDS ${RC_OUT_FILE}
    )
endif()

add_executable(${PROJECT_NAME} ${SOURCES})

if (APPLE)
    target_link_libraries(
        ${PROJECT_NAME}
    )
else()
    target_link_libraries(
        ${PROJECT_NAME}
        ${RC_OUT_FILE}
    )
endif()

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)
