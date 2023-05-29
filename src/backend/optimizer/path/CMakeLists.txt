#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_path_SRC)

set(TGT_path_INC
        ${CMAKE_SOURCE_DIR}/include
        )

set(path_DEF_OPTIONS ${MACRO_OPTIONS})
set(path_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(path_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_optimizer_path TGT_path_SRC TGT_path_INC "${path_DEF_OPTIONS}" "${path_COMPILE_OPTIONS}" "${path_LINK_OPTIONS}")

