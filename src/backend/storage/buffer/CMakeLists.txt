#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_buffer_SRC)

set(TGT_buffer_INC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/port/${PORTNAME}
        )

set(buffer_DEF_OPTIONS ${MACRO_OPTIONS})
set(buffer_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(buffer_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_storage_buffer TGT_buffer_SRC TGT_buffer_INC "${buffer_DEF_OPTIONS}" "${buffer_COMPILE_OPTIONS}" "${buffer_LINK_OPTIONS}")

