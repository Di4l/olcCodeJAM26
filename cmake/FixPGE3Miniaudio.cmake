function(fix_pge3_miniaudio PGE3_SOURCE_DIR)
    # Assign input and output files
    set(FILE_TO_FIX "${PGE3_SOURCE_DIR}/extensions/miniaudio/olcPGEX3_Miniaudio.h")

    if(NOT EXISTS "${FILE_TO_FIX}")
        message(FATAL_ERROR "PGE3 Miniaudio header not found: ${FILE_TO_FIX}")
    endif()

    # -------------------------------------------------------------------------
    # Read original header
    # -------------------------------------------------------------------------
    file(READ "${FILE_TO_FIX}" CONTENT)

    # -------------------------------------------------------------------------
    # Apply workaround
    # -------------------------------------------------------------------------

    # Define the broken piece of code
    set(BROKEN_CODE
"		uint8_t* result = reinterpret_cast<uint8_t*>(std::memcpy(m_buffer.data(), data, m_buffer.size()));
		if(result == m_buffer.data())
			return false;"
    )
    # And the fix
    set(FIXED_CODE
"		uint8_t* result = reinterpret_cast<uint8_t*>(std::memcpy(m_buffer.data(), data, m_buffer.size()));
		// if(result == m_buffer.data())
		//	return false;"
    )

    string(FIND "${CONTENT}" "${BROKEN_CODE}" POS)

    if(POS EQUAL -1)
        message(WARNING "PGE3 Miniaudio workaround: expected code was not found in: ${FILE_TO_FIX}")
    endif()

    string(REPLACE
        "${BROKEN_CODE}"
        "${FIXED_CODE}"
        CONTENT
        "${CONTENT}"
    )

    # -------------------------------------------------------------------------
    # Write patched copy
    # -------------------------------------------------------------------------
    file(WRITE "${FILE_TO_FIX}" "${CONTENT}")

    message(STATUS "PGE3 Miniaudio: Fixed olc miniaudio extension file")
endfunction()
