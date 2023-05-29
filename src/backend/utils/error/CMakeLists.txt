#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_error_SRC)

set(TGT_error_INC
        ${CMAKE_SOURCE_DIR}/include
        )

set(error_DEF_OPTIONS ${MACRO_OPTIONS})
set(error_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(error_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_utils_error TGT_error_SRC TGT_error_INC "${error_DEF_OPTIONS}" "${error_COMPILE_OPTIONS}" "${error_LINK_OPTIONS}")

