execute_process(
        COMMAND bison -d -o y.tab.c bootparse.y
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

execute_process(
        COMMAND bash -c "sed -f boot.sed < y.tab.c > bootparse.c"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
execute_process(
        COMMAND mv y.tab.h bootstrap_tokens.h
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
execute_process(
        COMMAND rm -f y.tab.c
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

execute_process(
        COMMAND flex bootscanner.l
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
execute_process(
        COMMAND bash -c "sed -f boot.sed < lex.yy.c > bootscanner.c"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
execute_process(
        COMMAND rm -f lex.yy.c
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

execute_process(
        COMMAND mv ${CMAKE_CURRENT_SOURCE_DIR}/bootstrap_tokens.h ${CMAKE_SOURCE_DIR}/include/bootstrap/bootstrap_tokens.h
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set(TGT_bootstrap_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/bootscanner.c
        ${CMAKE_CURRENT_SOURCE_DIR}/bootparse.c
        ${CMAKE_CURRENT_SOURCE_DIR}/bootstrap.c
        )

set(TGT_bootstrap_INC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/port/${PORTNAME}
        )

set(bootstrap_DEF_OPTIONS ${MACRO_OPTIONS})
set(bootstrap_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS} -fPIC -fvisibility=hidden)
set(bootstrap_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_backend_bootstrap TGT_bootstrap_SRC TGT_bootstrap_INC "${bootstrap_DEF_OPTIONS}" "${bootstrap_COMPILE_OPTIONS}" "${bootstrap_LINK_OPTIONS}")
