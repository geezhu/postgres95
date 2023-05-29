#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_common_SRC)

set(TGT_common_INC
        ${CMAKE_SOURCE_DIR}/include
        )

set(common_DEF_OPTIONS ${MACRO_OPTIONS})
set(common_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(common_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_access_common TGT_common_SRC TGT_common_INC "${common_DEF_OPTIONS}" "${common_COMPILE_OPTIONS}" "${common_LINK_OPTIONS}")

