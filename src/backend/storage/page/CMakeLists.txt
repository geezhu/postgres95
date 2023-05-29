#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_page_SRC)

set(TGT_page_INC
        ${CMAKE_SOURCE_DIR}/include
        )

set(page_DEF_OPTIONS ${MACRO_OPTIONS})
set(page_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(page_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_storage_page TGT_page_SRC TGT_page_INC "${page_DEF_OPTIONS}" "${page_COMPILE_OPTIONS}" "${page_LINK_OPTIONS}")

