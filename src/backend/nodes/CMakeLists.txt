#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_nodes_SRC)

set(TGT_nodes_INC
        ${CMAKE_SOURCE_DIR}/include
        )

set(nodes_DEF_OPTIONS ${MACRO_OPTIONS})
set(nodes_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(nodes_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_backend_nodes TGT_nodes_SRC TGT_nodes_INC "${nodes_DEF_OPTIONS}" "${nodes_COMPILE_OPTIONS}" "${nodes_LINK_OPTIONS}")

