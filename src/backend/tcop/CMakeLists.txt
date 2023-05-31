#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_tcop_SRC)

set(TGT_tcop_INC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/port/${PORTNAME}
        )

set(tcop_DEF_OPTIONS ${MACRO_OPTIONS})
set(tcop_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(tcop_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_backend_tcop TGT_tcop_SRC TGT_tcop_INC "${tcop_DEF_OPTIONS}" "${tcop_COMPILE_OPTIONS}" "${tcop_LINK_OPTIONS}")

