cmake_minimum_required(VERSION 3.24)

project(olcCodeJAM26
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================
# olcPixelGameEngine3
# ============================================================

include(FetchContent)

FetchContent_Declare(
    olcPixelGameEngine3
    GIT_REPOSITORY https://github.com/OneLoneCoder/olcPixelGameEngine3.git
    GIT_TAG main
    SOURCE_SUBDIR dev
)

FetchContent_MakeAvailable(olcPixelGameEngine3)

# FetchContent_GetProperties() populates these variables.
FetchContent_GetProperties(olcPixelGameEngine3)

message(STATUS "PGE3 SOURCE: ${olcpixelgameengine3_SOURCE_DIR}")
message(STATUS "PGE3 BINARY: ${olcpixelgameengine3_BINARY_DIR}")

# ============================================================
# PGE3 static library
# ============================================================
add_library(pge3_static STATIC
    src/pge3.cpp
)

target_include_directories(pge3_static
    PRIVATE
        ${olcpixelgameengine3_SOURCE_DIR}
)

# The generated header must exist before pge3.cpp is compiled.
add_dependencies(pge3_static
    olcPixelGameEngine3
)

# ============================================================
# Game
# ============================================================
set(GAME_HEADERS
    src/game.hpp
)

set(GAME_SOURCES
    src/game.cpp
    src/main.cpp
)

add_executable(${PROJECT_NAME}
    ${GAME_HEADERS}
    ${GAME_SOURCES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        src
        ${olcpixelgameengine3_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        pge3_static
)

# ============================================================
# Linux dependencies
# ============================================================

if(UNIX AND NOT APPLE)

    find_package(PNG REQUIRED)
    find_package(OpenGL REQUIRED)
    find_package(Threads REQUIRED)

    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            PNG::PNG
            OpenGL::GL
            Threads::Threads
            X11
            Xi
    )

endif()

# ============================================================
# Warnings
# ============================================================

if(MSVC)

    target_compile_options(${PROJECT_NAME}
        PRIVATE
            /W4
    )

else()

    target_compile_options(${PROJECT_NAME}
        PRIVATE
            -Wall
            -Wextra
    )

endif()