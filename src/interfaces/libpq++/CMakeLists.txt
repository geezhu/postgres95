#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_libpq++_SRC)

set(TGT_libpq++_INC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}
        )

set(libpq++_DEF_OPTIONS ${MACRO_OPTIONS})
set(libpq++_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(libpq++_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_interfaces_libpq++ TGT_libpq++_SRC TGT_libpq++_INC "${libpq++_DEF_OPTIONS}" "${libpq++_COMPILE_OPTIONS}" "${libpq++_LINK_OPTIONS}")
set_target_properties(postgres_interfaces_libpq++ PROPERTIES LINKER_LANGUAGE CXX)
