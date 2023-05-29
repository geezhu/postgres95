#This is the main CMAKE for build bin.
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} TGT_heap_SRC)

set(TGT_heap_INC
        ${CMAKE_SOURCE_DIR}/include
        )

set(heap_DEF_OPTIONS ${MACRO_OPTIONS})
set(heap_COMPILE_OPTIONS ${OPTIMIZE_OPTIONS} ${OS_OPTIONS} ${PROTECT_OPTIONS} ${WARNING_OPTIONS} ${BIN_SECURE_OPTIONS} ${CHECK_OPTIONS})
set(heap_LINK_OPTIONS ${BIN_LINK_OPTIONS})
add_static_objtarget(postgres_access_heap TGT_heap_SRC TGT_heap_INC "${heap_DEF_OPTIONS}" "${heap_COMPILE_OPTIONS}" "${heap_LINK_OPTIONS}")

