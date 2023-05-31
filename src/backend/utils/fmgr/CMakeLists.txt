#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_fmgr_SRC)

set(TGT_fmgr_INC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/port/${PORTNAME}
        )

set(fmgr_DEF_OPTIONS ${MACRO_OPTIONS})
set(fmgr_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(fmgr_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_utils_fmgr TGT_fmgr_SRC TGT_fmgr_INC "${fmgr_DEF_OPTIONS}" "${fmgr_COMPILE_OPTIONS}" "${fmgr_LINK_OPTIONS}")

