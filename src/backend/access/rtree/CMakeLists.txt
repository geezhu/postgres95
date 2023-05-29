#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_rtree_SRC)

set(TGT_rtree_INC
        ${CMAKE_SOURCE_DIR}/include
        )

set(rtree_DEF_OPTIONS ${MACRO_OPTIONS})
set(rtree_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(rtree_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_access_rtree TGT_rtree_SRC TGT_rtree_INC "${rtree_DEF_OPTIONS}" "${rtree_COMPILE_OPTIONS}" "${rtree_LINK_OPTIONS}")

