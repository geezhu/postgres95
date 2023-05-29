#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_regex_SRC)

set(TGT_regex_INC
        ${CMAKE_SOURCE_DIR}/include
        )

set(regex_DEF_OPTIONS ${MACRO_OPTIONS})
set(regex_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(regex_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_backend_regex TGT_regex_SRC TGT_regex_INC "${regex_DEF_OPTIONS}" "${regex_COMPILE_OPTIONS}" "${regex_LINK_OPTIONS}")

