#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_transam_SRC)

set(TGT_transam_INC
        ${CMAKE_SOURCE_DIR}/include
        )

set(transam_DEF_OPTIONS ${MACRO_OPTIONS})
set(transam_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(transam_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_access_transam TGT_transam_SRC TGT_transam_INC "${transam_DEF_OPTIONS}" "${transam_COMPILE_OPTIONS}" "${transam_LINK_OPTIONS}")

