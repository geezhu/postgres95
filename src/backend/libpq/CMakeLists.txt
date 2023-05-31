#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_libpq_SRC)

set(TGT_libpq_INC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/port/${PORTNAME}
        )

set(libpq_DEF_OPTIONS ${MACRO_OPTIONS} -DPOSTPORT='"5432"')
set(libpq_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(libpq_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_backend_libpq TGT_libpq_SRC TGT_libpq_INC "${libpq_DEF_OPTIONS}" "${libpq_COMPILE_OPTIONS}" "${libpq_LINK_OPTIONS}")

