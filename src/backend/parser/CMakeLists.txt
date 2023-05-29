#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_parser_SRC)

set(TGT_parser_INC
        ${CMAKE_SOURCE_DIR}/include
        )

set(parser_DEF_OPTIONS ${MACRO_OPTIONS})
set(parser_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(parser_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_backend_parser TGT_parser_SRC TGT_parser_INC "${parser_DEF_OPTIONS}" "${parser_COMPILE_OPTIONS}" "${parser_LINK_OPTIONS}")

