#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_lib_SRC)

set(TGT_lib_INC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/port/${PORTNAME}
        )

set(lib_DEF_OPTIONS ${MACRO_OPTIONS})
set(lib_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(lib_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_backend_lib TGT_lib_SRC TGT_lib_INC "${lib_DEF_OPTIONS}" "${lib_COMPILE_OPTIONS}" "${lib_LINK_OPTIONS}")

