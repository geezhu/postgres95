#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_init_SRC)

set(TGT_init_INC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/port/${PORTNAME}
        )

set(init_DEF_OPTIONS ${MACRO_OPTIONS} -DPOSTGRESDIR="${POSTGRESDIR}" -DPGDATADIR="${DATADIR}")
set(init_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(init_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_utils_init TGT_init_SRC TGT_init_INC "${init_DEF_OPTIONS}" "${init_COMPILE_OPTIONS}" "${init_LINK_OPTIONS}")

