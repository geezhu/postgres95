#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_lmgr_SRC)

set(TGT_lmgr_INC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/port/${PORTNAME}
        )

set(lmgr_DEF_OPTIONS ${MACRO_OPTIONS})
set(lmgr_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(lmgr_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_storage_lmgr TGT_lmgr_SRC TGT_lmgr_INC "${lmgr_DEF_OPTIONS}" "${lmgr_COMPILE_OPTIONS}" "${lmgr_LINK_OPTIONS}")

