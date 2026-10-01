function(embed_asset TARGET ASSET_FILE ASSET_NAME)
    # -------------------------------------------------------------------------
    # Validate arguments
    # -------------------------------------------------------------------------

    if(NOT TARGET ${TARGET})
        message(FATAL_ERROR "embed_asset(): target '${TARGET}' does not exist")
    endif()

    if(NOT EXISTS "${ASSET_FILE}")
        message(FATAL_ERROR "embed_asset(): asset '${ASSET_FILE}' does not exist")
    endif()

    # -------------------------------------------------------------------------
    # Read asset
    # -------------------------------------------------------------------------
    file(READ "${ASSET_FILE}" ASSET_HEX HEX)

    string(LENGTH "${ASSET_HEX}" ASSET_HEX_LENGTH)
    math(EXPR ASSET_SIZE "${ASSET_HEX_LENGTH} / 2")

    # Convert:
    #   89504E47...
    # into:
    #   0x89, 0x50, 0x4E, 0x47, ...
    string(REGEX REPLACE
        "([0-9A-Fa-f][0-9A-Fa-f])"
        "0x\\1, "
        ASSET_BYTES
        "${ASSET_HEX}"
    )

    # -------------------------------------------------------------------------
    # Output
    # -------------------------------------------------------------------------
    set(OUTPUT_DIR  "${CMAKE_BINARY_DIR}/generated/assets")
    set(OUTPUT_FILE "${OUTPUT_DIR}/${ASSET_NAME}.hpp")

    file(MAKE_DIRECTORY "${OUTPUT_DIR}")

    file(WRITE "${OUTPUT_FILE}"
"//-----------------------------------------------------------------------------
//-- Generated file. Do not edit.

#pragma once

#include <array>
#include <cstdint>

namespace assets
{
    inline constexpr std::array<std::uint8_t, ${ASSET_SIZE}> ${ASSET_NAME}{
        ${ASSET_BYTES}
    };
}
")

    # -------------------------------------------------------------------------
    # Make generated header available to the target
    # -------------------------------------------------------------------------
    target_include_directories(${TARGET}
        PRIVATE
            "${OUTPUT_DIR}"
    )

    # Tell IDEs/build systems that this generated file belongs to the target.
    target_sources(${TARGET}
        PRIVATE
            "${OUTPUT_FILE}"
    )

    message(STATUS
        "Embedded asset: ${ASSET_FILE} -> ${ASSET_NAME} (${ASSET_SIZE} bytes)"
    )

endfunction()