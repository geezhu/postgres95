#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_sort_SRC)

set(TGT_sort_INC
        ${CMAKE_SOURCE_DIR}/include
        )

set(sort_DEF_OPTIONS ${MACRO_OPTIONS})
set(sort_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(sort_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_utils_sort TGT_sort_SRC TGT_sort_INC "${sort_DEF_OPTIONS}" "${sort_COMPILE_OPTIONS}" "${sort_LINK_OPTIONS}")

