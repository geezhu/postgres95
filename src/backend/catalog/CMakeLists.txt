#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_catalog_SRC)

set(TGT_catalog_INC
        ${CMAKE_SOURCE_DIR}/include
        )

set(catalog_DEF_OPTIONS ${MACRO_OPTIONS})
set(catalog_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(catalog_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_catalog TGT_catalog_SRC TGT_catalog_INC "${catalog_DEF_OPTIONS}" "${catalog_COMPILE_OPTIONS}" "${catalog_LINK_OPTIONS}")

