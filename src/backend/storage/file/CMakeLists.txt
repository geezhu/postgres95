#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_file_SRC)

set(TGT_file_INC
        ${CMAKE_SOURCE_DIR}/include
        )

set(file_DEF_OPTIONS ${MACRO_OPTIONS})
set(file_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(file_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_storage_file TGT_file_SRC TGT_file_INC "${file_DEF_OPTIONS}" "${file_COMPILE_OPTIONS}" "${file_LINK_OPTIONS}")

