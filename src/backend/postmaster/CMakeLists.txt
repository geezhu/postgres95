#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_postmaster_SRC)

set(TGT_postmaster_INC
        ${CMAKE_SOURCE_DIR}/include
        )

set(postmaster_DEF_OPTIONS ${MACRO_OPTIONS})
set(postmaster_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(postmaster_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_backend_postmaster TGT_postmaster_SRC TGT_postmaster_INC "${postmaster_DEF_OPTIONS}" "${postmaster_COMPILE_OPTIONS}" "${postmaster_LINK_OPTIONS}")

