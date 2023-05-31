#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_libpgtcl_SRC)

set(TGT_libpgtcl_INC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src/interfaces/libpq
        ${TCL_INCDIR}
        )

set(libpgtcl_DEF_OPTIONS ${MACRO_OPTIONS})
set(libpgtcl_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(libpgtcl_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_interfaces_libpgtcl TGT_libpgtcl_SRC TGT_libpgtcl_INC "${libpgtcl_DEF_OPTIONS}" "${libpgtcl_COMPILE_OPTIONS}" "${libpgtcl_LINK_OPTIONS}")

