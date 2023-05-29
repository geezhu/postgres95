#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_mmgr_SRC)

set(TGT_mmgr_INC
        ${CMAKE_SOURCE_DIR}/include
        )

set(mmgr_DEF_OPTIONS ${MACRO_OPTIONS})
set(mmgr_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(mmgr_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_utils_mmgr TGT_mmgr_SRC TGT_mmgr_INC "${mmgr_DEF_OPTIONS}" "${mmgr_COMPILE_OPTIONS}" "${mmgr_LINK_OPTIONS}")

