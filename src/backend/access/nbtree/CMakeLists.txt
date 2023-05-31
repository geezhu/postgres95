#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_nbtree_SRC)

set(TGT_nbtree_INC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/port/${PORTNAME}
        )

set(nbtree_DEF_OPTIONS ${MACRO_OPTIONS})
set(nbtree_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(nbtree_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_access_nbtree TGT_nbtree_SRC TGT_nbtree_INC "${nbtree_DEF_OPTIONS}" "${nbtree_COMPILE_OPTIONS}" "${nbtree_LINK_OPTIONS}")

