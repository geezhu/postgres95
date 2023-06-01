#This is the main CMAKE for build bin.

execute_process(
        COMMAND bison -d -o y.tab.c gram.y
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

execute_process(
        COMMAND mv y.tab.c gram.c
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

execute_process(
        COMMAND mv y.tab.h parse.h
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)


execute_process(
        COMMAND flex scan.l
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

execute_process(
        COMMAND mv lex.yy.c scan.c
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

execute_process(
        COMMAND mv ${CMAKE_CURRENT_SOURCE_DIR}/parse.h ${CMAKE_SOURCE_DIR}/include/parser/parse.h
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set(TGT_parser_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/analyze.c
        ${CMAKE_CURRENT_SOURCE_DIR}/catalog_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/dbcommands.c
        ${CMAKE_CURRENT_SOURCE_DIR}/keywords.c
        ${CMAKE_CURRENT_SOURCE_DIR}/parse_query.c
        ${CMAKE_CURRENT_SOURCE_DIR}/parser.c
        ${CMAKE_CURRENT_SOURCE_DIR}/scansup.c
        ${CMAKE_CURRENT_SOURCE_DIR}/scan.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gram.c
        )

set(TGT_parser_INC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/port/${PORTNAME}
        )

set(parser_DEF_OPTIONS ${MACRO_OPTIONS})
set(parser_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(parser_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_backend_parser TGT_parser_SRC TGT_parser_INC "${parser_DEF_OPTIONS}" "${parser_COMPILE_OPTIONS}" "${parser_LINK_OPTIONS}")

