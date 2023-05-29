#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_large_object_SRC)

set(TGT_large_object_INC
        ${CMAKE_SOURCE_DIR}/include
        )

set(large_object_DEF_OPTIONS ${MACRO_OPTIONS})
set(large_object_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(large_object_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_storage_large_object TGT_large_object_SRC TGT_large_object_INC "${large_object_DEF_OPTIONS}" "${large_object_COMPILE_OPTIONS}" "${large_object_LINK_OPTIONS}")

