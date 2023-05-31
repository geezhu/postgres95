#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_index_SRC)

set(TGT_index_INC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/port/${PORTNAME}
        )

set(index_DEF_OPTIONS ${MACRO_OPTIONS})
set(index_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(index_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_access_index TGT_index_SRC TGT_index_INC "${index_DEF_OPTIONS}" "${index_COMPILE_OPTIONS}" "${index_LINK_OPTIONS}")

