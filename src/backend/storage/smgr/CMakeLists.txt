#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_smgr_SRC)

set(TGT_smgr_INC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/port/${PORTNAME}
        )

set(smgr_DEF_OPTIONS ${MACRO_OPTIONS})
set(smgr_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(smgr_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_storage_smgr TGT_smgr_SRC TGT_smgr_INC "${smgr_DEF_OPTIONS}" "${smgr_COMPILE_OPTIONS}" "${smgr_LINK_OPTIONS}")

