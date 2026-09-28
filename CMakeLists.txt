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
# Game
# ============================================================

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        olcPixelGameEngine3
)

# olcPixelGameEngine3.h is generated/copied to the root
# of the fetched PGE3 source tree.
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${olcpixelgameengine3_SOURCE_DIR}
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