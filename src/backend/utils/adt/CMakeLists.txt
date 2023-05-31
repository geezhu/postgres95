#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_adt_SRC)

set(TGT_adt_INC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/port/${PORTNAME}
        )

set(adt_DEF_OPTIONS ${MACRO_OPTIONS} -DPORTNAME_linux)
set(adt_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(adt_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_utils_adt TGT_adt_SRC TGT_adt_INC "${adt_DEF_OPTIONS}" "${adt_COMPILE_OPTIONS}" "${adt_LINK_OPTIONS}")

