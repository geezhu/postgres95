#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_tioga_SRC)

set(TGT_tioga_INC
        ${CMAKE_SOURCE_DIR}/include
        )

set(tioga_DEF_OPTIONS ${MACRO_OPTIONS})
set(tioga_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(tioga_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_backend_tioga TGT_tioga_SRC TGT_tioga_INC "${tioga_DEF_OPTIONS}" "${tioga_COMPILE_OPTIONS}" "${tioga_LINK_OPTIONS}")

