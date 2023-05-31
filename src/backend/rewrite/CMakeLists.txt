#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_rewrite_SRC)

set(TGT_rewrite_INC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/port/${PORTNAME}
        )

set(rewrite_DEF_OPTIONS ${MACRO_OPTIONS})
set(rewrite_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(rewrite_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_backend_rewrite TGT_rewrite_SRC TGT_rewrite_INC "${rewrite_DEF_OPTIONS}" "${rewrite_COMPILE_OPTIONS}" "${rewrite_LINK_OPTIONS}")

