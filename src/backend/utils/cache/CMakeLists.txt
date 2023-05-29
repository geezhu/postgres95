#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_cache_SRC)

set(TGT_cache_INC
        ${CMAKE_SOURCE_DIR}/include
        )

set(cache_DEF_OPTIONS ${MACRO_OPTIONS})
set(cache_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(cache_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_utils_cache TGT_cache_SRC TGT_cache_INC "${cache_DEF_OPTIONS}" "${cache_COMPILE_OPTIONS}" "${cache_LINK_OPTIONS}")

