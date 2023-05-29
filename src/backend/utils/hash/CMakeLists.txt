#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_hash_SRC)

set(TGT_hash_INC
        ${CMAKE_SOURCE_DIR}/include
        )

set(hash_DEF_OPTIONS ${MACRO_OPTIONS})
set(hash_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(hash_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_utils_hash TGT_hash_SRC TGT_hash_INC "${hash_DEF_OPTIONS}" "${hash_COMPILE_OPTIONS}" "${hash_LINK_OPTIONS}")

