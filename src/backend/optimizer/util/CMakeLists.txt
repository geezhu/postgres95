#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_util_SRC)

set(TGT_util_INC
        ${CMAKE_SOURCE_DIR}/include
        )

set(util_DEF_OPTIONS ${MACRO_OPTIONS})
set(util_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(util_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_optimizer_util TGT_util_SRC TGT_util_INC "${util_DEF_OPTIONS}" "${util_COMPILE_OPTIONS}" "${util_LINK_OPTIONS}")

