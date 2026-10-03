cmake_minimum_required(VERSION 3.24)

project(olcCodeJAM26
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# ============================================================
# Make available a function to embed assets
# ============================================================
include("${CMAKE_SOURCE_DIR}/cmake/EmbedAsset.cmake")

# ============================================================
# Use spdlog for logging
# ============================================================
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.17.0
)

FetchContent_MakeAvailable(spdlog)

# ============================================================
# Miniaudio
# ============================================================

set(MINIAUDIO_DIR "${CMAKE_BINARY_DIR}/_deps/miniaudio")

file(DOWNLOAD
    "https://raw.githubusercontent.com/mackron/miniaudio/master/miniaudio.h"
    "${MINIAUDIO_DIR}/miniaudio.h"
    TLS_VERIFY ON
)

# ============================================================
# olcPixelGameEngine3
# ============================================================

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

# BEGIN: Apply patch on the miniaudio extension of the engine
include("${CMAKE_SOURCE_DIR}/cmake/FixPGE3Miniaudio.cmake")

fix_pge3_miniaudio(
    "${olcpixelgameengine3_SOURCE_DIR}"
)
# END: Apply patch

# ============================================================
# PGE3 static library
# ============================================================
add_library(pge3_static STATIC
    src/pge3.cpp
)

target_include_directories(pge3_static PRIVATE
    ${olcpixelgameengine3_SOURCE_DIR}
    # The miniaudio extension olcPGEX3_Miniaudio.h requires
    ${olcpixelgameengine3_SOURCE_DIR}/extensions/miniaudio
    ${MINIAUDIO_DIR}
)

# The generated header must exist before pge3.cpp is compiled.
add_dependencies(pge3_static
    olcPixelGameEngine3
)

# ============================================================
# Game
# ============================================================
set(GAME_HEADERS
    src/logger.hpp
    src/menu.hpp
    src/game.hpp
)

set(GAME_SOURCES
    src/logger.cpp
    src/menu.cpp
    src/game.cpp
    src/main.cpp
)

add_executable(${PROJECT_NAME}
    ${GAME_HEADERS}
    ${GAME_SOURCES}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG>
)

target_include_directories(${PROJECT_NAME} PRIVATE
    src
    ${olcpixelgameengine3_SOURCE_DIR}
    ${olcpixelgameengine3_SOURCE_DIR}/extensions/miniaudio
    ${MINIAUDIO_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    spdlog::spdlog
    pge3_static
)

# Embed the assets into the binary
embed_asset(${PROJECT_NAME}
    "${CMAKE_SOURCE_DIR}/assets/retro_blop_18.wav"
    retro_blop_18
)

embed_asset(${PROJECT_NAME}
    "${CMAKE_SOURCE_DIR}/assets/keyboard_001.wav"
    keyboard001
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