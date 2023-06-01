#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_pg_id_SRC)

set(TGT_pg_id_INC
        ${CMAKE_SOURCE_DIR}/include
        )

set(pg_id_DEF_OPTIONS ${MACRO_OPTIONS})
set(pg_id_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(pg_id_LINK_OPTIONS ${BIN_LINK_OPTIONS})
set(pg_id_LINK_LIST "")
add_bintarget(pg_id TGT_pg_id_SRC TGT_pg_id_INC "${pg_id_DEF_OPTIONS}" "${pg_id_COMPILE_OPTIONS}" "${pg_id_LINK_OPTIONS}" "${pg_id_LINK_LIST}")

