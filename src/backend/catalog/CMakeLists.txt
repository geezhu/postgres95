#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_catalog_SRC)
set(GLOBALBKI_SRCS_S 
        "@pg_database.h @pg_demon.h @pg_magic.h @pg_defaults.h @pg_variable.h @pg_server.h @pg_user.h @pg_hosts.h @pg_group.h @pg_log.h @pg_time.h"
        )

string(REPLACE "@" "${CMAKE_SOURCE_DIR}/include/catalog/" GLOBALBKI_SRCS ${GLOBALBKI_SRCS_S})

set(LOCALBKI_SRCS_S 
        "@pg_proc.h @pg_type.h @pg_attribute.h @pg_class.h @pg_inherits.h @pg_index.h @pg_version.h @pg_statistic.h @pg_operator.h @pg_opclass.h @pg_am.h @pg_amop.h @pg_amproc.h @pg_language.h @pg_parg.h @pg_aggregate.h @pg_ipl.h @pg_inheritproc.h @pg_rewrite.h @pg_listener.h @indexing.h"
        )
string(REPLACE "@" "${CMAKE_SOURCE_DIR}/include/catalog/" LOCALBKI_SRCS ${LOCALBKI_SRCS_S})

execute_process(
        COMMAND bash -c "sh genbki.sh ${BKIOPTS} ${GLOBALBKI_SRCS} > global1.bki"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
execute_process(
        COMMAND bash -c "sh genbki.sh ${BKIOPTS} ${LOCALBKI_SRCS} > local1_template1.bki"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set(TGT_catalog_INC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/port/${PORTNAME}
        )

set(catalog_DEF_OPTIONS ${MACRO_OPTIONS})
set(catalog_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(catalog_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_catalog TGT_catalog_SRC TGT_catalog_INC "${catalog_DEF_OPTIONS}" "${catalog_COMPILE_OPTIONS}" "${catalog_LINK_OPTIONS}")

